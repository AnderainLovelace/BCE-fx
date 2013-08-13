/*
 *	BCE Kernel
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
#include "../include/bool.h"
#include "../include/bce.h"
#include "../include/fraction.h"
#include "../include/equation.h"
#include "../include/matrix_io.h"
#include "../include/syntax.h"
#include "../include/polynomial.h"
#include "../include/fix.h"

/*
 *	balance_chemical_equation()
 *
 *	Balance a chemical equation.
 *
 *	@nptr: the formula of the chemical equation
 *	@count: the count of balance results will be put in this variable
 */
exp* balance_chemical_equation(char *nptr, int *count) {
	int mx, my;
	fact **mtx;
	exp *ret;

	mtx = get_balance_matrix(nptr, &mx, &my);
	if (!mtx)
		return(NULL);

	ret = solve_equations(mtx, mx, my, 0, 0, 0);
	if (!ret) {
		free_matrix(mtx, my);
		return(NULL);
	}

	if (!check_equation_result(mtx, ret, mx, my)) {
		free_matrix(mtx, my);
		free_expression_stack(ret, mx - 1);
		return(NULL);
	}

	free_matrix(mtx, my);

	finishing_expression_stack(ret, mx - 1);
	expression_bce_setx2one(ret, mx - 1);
	expression_to_number(ret, mx - 1, true);

	*count = mx - 1;

	return(ret);
}

/*
 *	automatic_balance_ce()
 *
 *	Balance a chemical equation and return the result automatically.
 *
 *	@nptr: the formula of the chemical equation
 */
char* automatic_balance_ce(char *nptr) {
	int c, fc;
	char *p;
	exp *r;
	fix *fx;

	r = balance_chemical_equation(nptr, &c);
	if (!r)
		return(NULL);

	fx = fix_syntax_ce(nptr, r, c, &fc);
	if (!fx) {
		free_expression_stack(r, c);
		return(NULL);
	}

	p = fix_redirect_sprint(fx, fc, strchr(nptr, SYNTAX_EQUAL) ? false : true);
	free_expression_stack(r, c);
	free(fx);
	if (!p)
		return(NULL);

	return(p);
}

