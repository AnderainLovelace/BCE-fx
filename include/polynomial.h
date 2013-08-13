/*
 *	Polynomial Subsystem
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


#ifndef _POLYNOMIAL_H_
	#include "include/bool.h"
	#include "include/fraction.h"
	#include "include/blockmem.h"

	/*  We can express a polynomial in this way:
	 *  Pn(x1...xn)=p0+p1x1+p2x2+...+pnxn
	 *  Pn(x1...xn) is a polynominal, each PnXn is one of its nodes.  */
	struct expressionItem {
		int flag;
		fact pfx;
	};
	typedef struct expressionItem expnode;

	struct expressionStack {
		int count;
		bmem stack;
		fact cst;
	};
	typedef struct expressionStack exp;

	/*  This data structure only works for 'finishing_expression_stack()' function.  */
	struct translateTable {
		int from;
		int to;
	};
	typedef struct translateTable exptrans;

	exp expression_create(int numerator, int denominator);
	exp fraction_to_expression(fact src);
	void free_expression(exp *ptr);
	void free_expression_stack(exp *src, int count);
	expnode* query_expression_node(bmem stack, int count, int flag);
	expnode* query_expression_node_ex(expnode *begin, expnode *end, int flag);
	expnode* get_insert_position(expnode *p, int count, int flag);
	bool push_expression_node(exp *target, expnode nd, factop op);
	bool push_expression_node_ex(exp *target, int flag, fact pfx, factop op);
	void push_expression_constant(exp *target, fact src, factop op);
	void expression_vf_basic(exp *target, fact src, factop op);
	void expression_plus(exp *target, fact src);
	void expression_minus(exp *target, fact src);
	void expression_multiplination(exp *target, fact src);
	void expression_division(exp *target, fact src);
	bool simplify_expression_node(exp *target);
	bool expression_double_operation(exp *exp1, factop op1, exp exp2, factop op2, fact ft);
	void finishing_expression_stack(exp *src, int len);
	bool expression_memcpy(exp *target, exp src, bool destroy_after_copy);
	void expression_bce_setx2one(exp *src, int len);
	void expression_to_number(exp *src, int len, bool inc_args);
	char *get_unknown_symbol(int id);
	char* sprint_expression(exp sz);

	#define EXPMODULE_BASEID 0

	#define _POLYNOMIAL_H_
#endif

