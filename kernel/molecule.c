/*
 *	Molecule Analyzer
 *
 *	Copyright (C) 2011-2012 XiaoJSoft Studio. All Rights Reserved.
 *	Copyright (C) Ji WenCong <whs_jwc@163.com>
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/bool.h"
#include "../include/bracket.h"
#include "../include/blockmem.h"
#include "../include/string_ext.h"
#include "../include/element.h"
#include "../include/molecule.h"

/*
 *	solve_molecule_hydrate()
 *
 *	Analyze the composition of a molecular formula with hydrate (e.g: "CuSO4.5H2O", "Na2S.Na2SO4.3H2O"...)
 *
 *	@begin, @end: pointer 'begin' points to the head of a string, pointer 'end' points to the end of the string.
 *	@eptr: storage pool of elements
 *	@ecount: counter of the storage pool 'eptr'
 *	@suffix: digits after or before the molecular formula
 */
bool solve_molecule_hydrate(char *begin, char *end, bmem *eptr, int *ecount, int suffix) {
	char *dot;
	int stack;

	for (stack = 0, dot = begin; begin <= end; begin++) {
		/*  Ignore brackets in the formula  */
		if (strchr(BRACKET_LEFT, *begin))
			stack++;
		if (strchr(BRACKET_RIGHT, *begin))
			stack--;
		if (stack)
			continue;
		/*  Split the molecular formula  */
		if (*begin == HYDRATE_DOT || begin == end) {
			if (!solve_molecule(dot, begin == end ? begin : begin - 1, eptr, ecount, suffix))
				return(false);
			dot = begin + 1;
		}
	}

	/*  Check the bracket stack  */
	if (stack)
		return(false);
	else
		return(true);
}

/*
 *	solve_molecule()
 *
 *	Analyze the composition of a molecular formula without hydrate
 *
 *	@begin, @end: pointer 'begin' points to the head of a string, pointer 'end' points to the end of the string.
 *	@eptr: storage pool of elements
 *	@ecount: counter of the storage pool 'eptr'
 *	@suffix: digits after or before the molecular formula
 */
bool solve_molecule(char *begin, char *end, bmem *eptr, int *ecount, int suffix) {
	char *ptr, *b_ptr, *temp, *start;
	int stack, prefix;
	element *eap_ptr;

	if (strplen(begin, end) <= 0)
		return(true);

	/*  Get the prefix number  */
	prefix = strptoi(begin, end, &temp);
	if (!temp)
		return(false);

	if (prefix <= 0)
		prefix = 1;

	/*  Fix the string  */
	if (temp != begin) {
		start = temp;
		suffix *= prefix;
	} else {
		start = begin;
	}

	if (strplen(start, end) <= 0)
		return(true);

	if (strpcomp(start, end, SPECIAL_SIGNATURE_SOLID) || strpcomp(start, end, SPECIAL_SIGNATURE_LIQUIT) || strpcomp(start, end, SPECIAL_SIGNATURE_GAS))
		return(true);

	/*  Recognize whether the string is a electronic expression  */
	if (strpcomp(start, end, ELECTRIC_E_PLUS) || strpcomp(start, end, ELECTRIC_E_MINUS)) {
		for (eap_ptr = (element*)eptr->ptr; eap_ptr < (element*)eptr->ptr + *ecount; eap_ptr++)
			if (strpcomp(eap_ptr->begin, eap_ptr->end, ELECTRIC_E_NAME)) {
				/*  Modify the value  */
				eap_ptr->count += strpcomp(start, end, ELECTRIC_E_PLUS) ? suffix : -suffix;
				return(true);
			}

		/*  Add the electronic descriptor in the element stack  */
		if (!reallocate_block_memory(eptr, (++(*ecount)) * sizeof(element)))
			return(false);

		eap_ptr = (element*)eptr->ptr + (*ecount) - 1;

		eap_ptr->begin = start;
		eap_ptr->end = start;
		eap_ptr->count = strpcomp(start, end, ELECTRIC_E_PLUS) ? suffix : -suffix;

		return(true);
	}

	/*  Solve the bracket  */
	for (stack = 0, ptr = start; ptr <= end; ptr++) {
		if (!strchr(BRACKET_LEFT, *ptr))
			continue;

		for (b_ptr = ptr; b_ptr <= end; b_ptr++) {
			if (strchr(BRACKET_LEFT, *b_ptr))
				stack++;

			if (strchr(BRACKET_RIGHT, *b_ptr))
				stack--;

			if (!stack) {
				/*  Solve the molecular on the left  */
				if (!solve_molecule_hydrate(start, ptr - 1, eptr, ecount, suffix))
					return(false);

				/*  Get the suffix after the bracket  */
				prefix = strptoi(b_ptr + 1, end, &temp);
				if (!temp)
					return(false);
				if (prefix <= 0)
					prefix = 1;

				/*  Solve the molecular in the bracket  */
				if (!solve_molecule_hydrate(ptr + 1, b_ptr - 1, eptr, ecount, suffix * prefix))
					return(false);

				/*  Solve the molecular on the right  */
				if (!solve_molecule_hydrate(temp, end, eptr, ecount, suffix))
					return(false);

				return (true);
			}
		}
	}

	/*  Look for the next upcase character  */
	for (ptr = start + 1; strplen(ptr, end) > 0 && (!isupper((int)(*ptr))); ptr++)
		;

	if (strplen(ptr, end) <= 0) {
		/*  Only one element, divide it into a symbol and a number  */
		for (ptr = start; strplen(ptr, end) >= 0; ptr++) {
			if (strpisnum(ptr, end) == true) {
				/*  Get the suffix number  */
				prefix = strptoi(ptr, end, NULL);
				if (prefix <= 0)
					prefix = 1;

				for (eap_ptr = (element*)eptr->ptr; eap_ptr < (element*)eptr->ptr + *ecount; eap_ptr++)
					if (strpqcomp(eap_ptr->begin, eap_ptr->end, start, ptr - 1)) {
						/*  Modify the value  */
						eap_ptr->count += suffix * prefix;
						return(true);
					}

				/*  Add the electronic descriptor in the element stack  */
				if (!reallocate_block_memory(eptr, (++(*ecount)) * sizeof(element)))
					return(false);
				eap_ptr = (element*)eptr->ptr + *ecount - 1;
				eap_ptr->begin = start;
				eap_ptr->end = ptr - 1;
				eap_ptr->count = prefix * suffix;

				break;
			}
		}
	} else {
		if (!solve_molecule_hydrate(start, ptr - 1, eptr, ecount, suffix))
			return(false);
		if (!solve_molecule_hydrate(ptr, end, eptr, ecount, suffix))
			return(false);
	}

	return(true);
}

