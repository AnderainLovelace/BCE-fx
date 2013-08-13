/*
 *	Intelligent Chemical Equation Formatter/Fixer (ifix)
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
#include "../include/bracket.h"
#include "../include/fix.h"
#include "../include/polynomial.h"
#include "../include/string_ext.h"
#include "../include/fraction.h"
#include "../include/kasprintf.h"
#include "../include/syntax.h"
#include "../include/bool.h"

/*
 *	fix_compare()
 *
 *	Comparer for qsort
 */
int fix_compare(const void *p1, const void *p2) {
	return(((fix*)p1)->side - ((fix*)p2)->side);
}

/*
 *	fix_syntax_ce()
 *
 *	To fix the chemical equation if it has some issues.
 *
 *	@nptr: the chemical equation
 *	@s: the result linear polynomials
 *	@count: the count of @s
 *	@fc: a pointer points to a integer variable which contains the count of the fix-table
 */
fix* fix_syntax_ce(char *nptr, exp *s, int count, int *fc) {
	fix *ret, *pf;
	int c, stack, idx, real;
	exp *pe;
	bool required;
	char *last, *current, *equal, *pc;

	/*  Intialize  */
	ret = NULL;
	required = true;
	c = 0;

	/*  To find the '=' character  */
	equal = strchr(nptr, SYNTAX_EQUAL);

	/*  Statistic valid solutions  */
	for (pe = s; pe < s + count; pe++) {
		if (!(pe->count == 0 && fraction_compare(F_ZERO, pe->cst) == 0)) {
			c++;
		}
		if (pe->count != 0)
			required = false;
	}

	if (!c)
		return(NULL);

	/*  Allocate the storage  */
	ret = (fix*) malloc(c * sizeof(fix));
	if (!ret)
		return(NULL);
	memset(ret, 0, c * sizeof(fix));

	/*  Build the fix-table  */
	for (pc = nptr, last = nptr, stack = 0, idx = 0, real = 0; *pc != 0; pc++) {
		if (strchr(BRACKET_LEFT, *pc))
			stack++;
		if (strchr(BRACKET_RIGHT, *pc))
			stack--;
		if (stack)
			continue;
		if (strchr(SYNTAX_SPLITER, *pc) != NULL || *(pc + 1) == '\0') {
			current = (*(pc + 1) == '\0') ? pc : pc - 1;
			if (!(s[real].count == 0 && fraction_compare(F_ZERO, s[real].cst) == 0)) {
				ret[idx].begin = last;
				ret[idx].end = current;
				ret[idx].num = s[real];
				if (equal)
					if (pc > equal)
						ret[idx].side = true;
				idx++;
			}
			real++;
			last = pc + 1;
		}
	}

	/*  Change the sign if it's necessary  */
	if (required)
		for (pf = ret; pf < ret + c; pf++)
			if (fraction_compare(pf->num.cst, F_ZERO) > 0) {
				pf->num.cst = fraction_minus(F_ZERO, pf->num.cst);
				pf->side = !(pf->side);

			}

	/*  Sort the fix-table  */
	qsort(ret, c, sizeof(fix), fix_compare);

	*fc = c;
	return(ret);
}

/*
 *	fix_redirect_sprint()
 *
 *	Translate a fix-table to a human-readable expression
 *
 *	@p: the fix-table
 *	@fc: the count of @p
 */
char* fix_redirect_sprint(fix *p, int fc, int exchangeable) {
	fix *pf;
	char *tmp, *symbol, *numpr, *swap, *sign_c;
	bool blast, wequ;

	/*  Allocate the storage  */
	tmp = (char*) malloc(sizeof(char));
	if (!tmp)
		return(NULL);
	*tmp = '\0';

	/*  Format  */
	for (pf = p, blast = false, wequ = false; pf < p + fc; pf++) {
		/*  Copy the symbol  */
		symbol = strpush(pf->begin, pf->end);
		if (!symbol) {
			free(tmp);
			return(NULL);
		}

		/*  Copy the prefix number  */
		numpr = sprint_expression(pf->num);
		if (!numpr) {
			free(tmp);
			free(symbol);
			return(NULL);
		}

		/*  Decide the signature  */
		if (pf < p + fc - 1) {
			if ((pf + 1)->side != blast) {
				blast = (pf + 1)->side;
				sign_c = exchangeable ? connector_opposite_equal : connector_equal;
				wequ = true;
			} else {
				sign_c = connector_plus;
			}
		} else {
			if (!wequ)
				sign_c = connector_nullrside;
			else
				sign_c = connector_null;
		}

		/*  Add to the formula  */
		if (strcmp(numpr, JMP_ONE) == 0)
			kasprintf(&swap, "%s%s%s", tmp, symbol, sign_c);
		else
			kasprintf(&swap, "%s(%s)%s%s", tmp, numpr, symbol, sign_c);

		free(numpr);
		free(symbol);
		free(tmp);

		if (!swap) {
			return(NULL);
		}

		tmp = swap;
	}

	if (*tmp == '=' || *tmp == '\0') {
		free(tmp);
		tmp = NULL;
	}

	return(tmp);
}

