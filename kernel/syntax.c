/*
 *	Chemical Equation Syntax Analyzer
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
#include "../include/bracket.h"
#include "../include/fraction.h"
#include "../include/polynomial.h"
#include "../include/string_ext.h"
#include "../include/molecule.h"
#include "../include/syntax.h"
#include "../include/kasprintf.h"
#include "../include/matrix_io.h"

/*
 *	get_balance_matrix()
 *
 *	Convert a chemical equation into matrix form.
 *
 *	@nptr: the formula of a chemical equation
 *	@high_x, @high_y: variables that will be used to contain the size of the result matrix
 *
 */
fact** get_balance_matrix(char *nptr, int *high_x, int *high_y) {
	fact *ip_ptr, *header, **ret;
	char *ptr, *equal, *dot;
	int c, ec, stack, solve_count, counter, idx;
	bmem solver, prefetch;
	element *p1, *p2;

	/*  Initialize  */
	c = 0;
	ec = 0;
	solver = empty_block_memory(DEFAULT_PAGE_SIZE);
	prefetch = empty_block_memory(DEFAULT_PAGE_SIZE);

	/*  Look for the separator  */
	equal = strchr(nptr, SYNTAX_EQUAL);

	for (stack = 0, dot = nptr, ptr = nptr; *ptr != '\0'; ptr++) {
		if (strchr(BRACKET_LEFT, *ptr))
			stack++;
		if (strchr(BRACKET_RIGHT, *ptr))
			stack--;
		if (stack)
			continue;
		if (strchr(SYNTAX_SPLITER, *ptr) != NULL || *(ptr + 1) == '\0') {
			if (!solve_molecule_hydrate(dot, (*(ptr + 1) == '\0') ? ptr : ptr - 1, &prefetch, &ec, 1)) {
				free_block_memory(&prefetch);
				return(NULL);
			}
			dot = ptr + 1;
			c++;
		}
	}
	if (!c) {
		free_block_memory(&prefetch);
		return(NULL);
	}

	/*  Allocate memory areas for the matrix  */
	ret = allocate_matrix(c + 1, ec);
	if (!ret) {
		free_block_memory(&prefetch);
		return(NULL);
	}
	for (header = get_matrix_header(ret, ec), ip_ptr = header; ip_ptr < header + (c + 1) * ec; ip_ptr++)
		*ip_ptr = fraction_create(0, 1);

	for (counter = 0, stack = 0, dot = nptr, ptr = nptr; *ptr != '\0'; ptr++) {
		if (strchr(BRACKET_LEFT, *ptr))
			stack++;
		if (strchr(BRACKET_RIGHT, *ptr))
			stack--;
		if (stack)
			continue;
		if (strchr(SYNTAX_SPLITER, *ptr) != NULL || *(ptr + 1) == '\0') {
			solve_count = 0;
			if (!solve_molecule_hydrate(dot, (*(ptr + 1) == '\0') ? ptr : ptr - 1, &solver, &solve_count, 1)) {
				free_block_memory(&solver);
				free_block_memory(&prefetch);
				free_matrix(ret, ec);
				return(NULL);
			}
			for (p1 = (element*)solver.ptr; p1 < (element*)solver.ptr + solve_count; p1++)
				for (p2 = (element*)prefetch.ptr, idx = 0; p2 < (element*)prefetch.ptr + ec; p2++, idx++)
					if (strpqcomp(p1->begin, p1->end, p2->begin, p2->end)) {
						if (!equal) {
							write_matrix(ret, counter, idx, fraction_create(p1->count, 1));
						} else {
							write_matrix(ret, counter, idx, fraction_create((dot < equal ? p1->count : -p1->count), 1));
						}
						break;
					}
			free_block_memory(&solver);
			dot = ptr + 1;
			counter++;
		}
	}
	free_block_memory(&prefetch);

	*high_x = c + 1;
	*high_y = ec;

	return(ret);
}

