/*
 *	Equation Solver
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
 *	along with this program.  If not, see <http:www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/bool.h"
#include "../include/blockmem.h"
#include "../include/matrix_io.h"
#include "../include/mmdiv.h"
#include "../include/polynomial.h"
#include "../include/equation.h"

/*
 *	solve_equations()
 *
 *	Solve linear equations or indeterminate equations.
 *
 *	@matrix: the matrix
 *	@mx: the number of columns of the matrix
 *	@my: the number of rows of the matrix
 *	@unknowns: the next unknown's id
 *	@base_offset_x, @base_offset_y: the offset of the sub matrix to be solved
 */
exp* solve_equations(fact **matrix, int mx, int my, int unknowns, int base_offset_x, int base_offset_y) {
	int idx;
	fact *temp, *ptr_equ, **ptr_matrix, *prepare, *pc;
	exp *ret, *rpptr, *solve_equ, *retptr, step;

	matrix += base_offset_y;

	/*  Exchange equations  */
	for (ptr_matrix = matrix; ptr_matrix < matrix + my; ptr_matrix++)
		if (fraction_compare(read_matrix(ptr_matrix, base_offset_x, 0), F_ZERO) != 0) {
			temp = *matrix;
			*matrix = *ptr_matrix;
			*ptr_matrix = temp;
			break;
		}

	/*  Allocate memory areas for the results  */
	ret = (exp*) malloc((mx - 1)*sizeof(exp));
	if (!ret)
		return(NULL);

	/*  CAUTION: this step is pretty important, the memory area of the new expression stack must be filled with zero.  */
	memset(ret, 0, (mx - 1) * sizeof(exp));

	/*  Nothing to exchange, remove the first column  */
	if (fraction_compare(read_matrix(matrix, base_offset_x, 0), F_ZERO) == 0) {
		*ret = expression_create(0, 1);
		/*  Push a new unknown symbol into the expression stack  */
		if (!push_expression_node_ex(ret, unknowns + 1, fraction_create(1, 1), fraction_plus)) {
			free_expression_stack(ret, mx - 1);
			return(NULL);
		}
		if (mx > 2) {
			/*  Remove the first column  */
			solve_equ = solve_equations(matrix, mx - 1, my, unknowns + 2, base_offset_x + 1, 0);
			if (!solve_equ) {
				free_expression_stack(ret, mx - 1);
				return(NULL);
			}
			for (rpptr = solve_equ, retptr = ret + 1, idx = 0; idx < mx - 2; idx++, rpptr++, retptr++)
				if (!expression_memcpy(retptr, *rpptr, false)) {
					free_expression_stack(ret, mx - 1);
					return(NULL);
				}
			free_expression_stack(solve_equ, mx - 2);

		}
		return(ret);
	}

	if (my == 1 || mx == 2) {
		/*  Only one unknown, get the result of it  */
		step = expression_create(matrix_offset(matrix, base_offset_x + mx - 1, 0)->numerator, matrix_offset(matrix, base_offset_x + mx - 1, 0)->denominator);
		for (rpptr = ret + 1, ptr_equ = matrix_offset(matrix, base_offset_x + 1, 0), pc = matrix_offset(matrix, base_offset_x + mx - 2, 0); ptr_equ <= pc; ptr_equ++, rpptr++) {
			*rpptr = expression_create(0, 1);
			/*  Generate a new unknown  */
			/*  and then copy values of other unknowns to the result  */
			if (push_expression_node_ex(rpptr, ++unknowns, fraction_create(1, 1), fraction_plus)) {
				if (!expression_double_operation(&step, fraction_minus, *rpptr, fraction_multiplination, *ptr_equ)) {
					free_expression_stack(ret, mx - 1);
					free_expression(&step);
					return(NULL);
				}
			} else {
				free_expression_stack(ret, mx - 1);
				return(NULL);
			}
		}
		expression_division(&step, read_matrix(matrix, base_offset_x, 0));

		if (!simplify_expression_node(&step)) {
			free_expression_stack(ret, mx - 1);
			free_expression(&step);
			return(NULL);
		}

		*ret = step;
	} else {
		/*  Have more than one unknown, eliminating  */
		/*  Allocate enough memory areas to contain new equations  */
		for (ptr_matrix = matrix; ptr_matrix < matrix + my; ptr_matrix++)
			if (fraction_compare(read_matrix(ptr_matrix, base_offset_x, 0), F_ZERO) != 0) {
				for (ptr_equ = matrix_offset(ptr_matrix, base_offset_x + mx - 1, 0), pc = matrix_offset(ptr_matrix, base_offset_x, 0); ptr_equ >= pc; ptr_equ--)
					*ptr_equ = fraction_division(*ptr_equ, read_matrix(ptr_matrix, base_offset_x, 0));
				if (ptr_matrix != matrix)
					for (prepare = matrix_offset(matrix, base_offset_x, 0), ptr_equ = matrix_offset(ptr_matrix, base_offset_x, 0); ptr_equ < matrix_offset(ptr_matrix, base_offset_x + mx, 0); prepare++, ptr_equ++)
						*ptr_equ = fraction_minus(*ptr_equ, *prepare);
			}

		/*  Generate the eliminated equations  */
		solve_equ = solve_equations(matrix, mx - 1, my - 1, unknowns, base_offset_x + 1, 1);
		if (!solve_equ) {
			free_expression_stack(ret, mx - 1);
			return(NULL);
		}

		/*  Calculate the value of the unknown in the first column  */
		step = expression_create(matrix_offset(matrix, base_offset_x + mx - 1, 0)->numerator, matrix_offset(matrix, base_offset_x + mx - 1, 0)->denominator);
		for (rpptr = solve_equ, ptr_equ = matrix_offset(matrix, base_offset_x + 1, 0); ptr_equ <= matrix_offset(matrix, base_offset_x + mx - 2, 0); rpptr++, ptr_equ++)
			if (!expression_double_operation(&step, fraction_minus, *rpptr, fraction_multiplination, *ptr_equ)) {
				free_expression_stack(ret, mx - 1);
				free_expression(&step);
				return(NULL);
			}

		if (!simplify_expression_node(&step)) {
			free_expression_stack(ret, mx - 1);
			free_expression(&step);
			return(NULL);
		}

		/*  Copy values of other unknowns to the result  */
		for (*ret = step, rpptr = solve_equ, retptr = ret + 1, idx = 0; idx < mx - 2; idx++, rpptr++, retptr++)
			if (!expression_memcpy(retptr, *rpptr, false)) {
				free_expression_stack(ret, mx - 1);
				return(NULL);
			}
		free_expression_stack(solve_equ, mx - 2);
	}

	return(ret);
}

/*
 *	check_equation_result()
 *
 *	Check the result returned by solve_equations().
 *
 *	@matrix: the matrix
 *	@ret: the results
 *	@mx: the number of columns of the matrix
 *	@my: the number of rows of the matrix
 */
bool check_equation_result(fact **matrix, exp *ret, int mx, int my) {
	fact *ptr_equ, **ptr_matrix;
	exp *rpptr, step;
	/*  Check results  */
	for (ptr_matrix = matrix; ptr_matrix < matrix + my; ptr_matrix++) {
		/*  Calculate the result of the equation  */
		step = expression_create(0, 1);
		for (ptr_equ = *ptr_matrix, rpptr = ret; ptr_equ < (*ptr_matrix) + mx - 1; ptr_equ++, rpptr++)
			if (!expression_double_operation(&step, fraction_plus, *rpptr, fraction_multiplination, *ptr_equ)) {
				free_expression(&step);
				return(false);
			}

		if (!simplify_expression_node(&step)) {
			free_expression(&step);
			return(false);
		}

		/*  Compare it with the constant of the equation  */
		if (!(step.count == 0 && fraction_compare(step.cst, *((*ptr_matrix) + mx - 1)) == 0)) {
			free_expression(&step);
			return(false);
		}
		free_expression(&step);
	}
	return(true);
}

