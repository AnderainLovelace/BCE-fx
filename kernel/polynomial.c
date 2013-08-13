/*
 *	Polynomial Subsystem with FMA
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
#include "../include/blockmem.h"
#include "../include/kasprintf.h"
#include "../include/fraction.h"
#include "../include/mmdiv.h"
#include "../include/polynomial.h"

/*  This table was used to generating unknown symbols  */
static char *unknown_table = "abcdefghijklmnopqrstuvwxyz";
#define unknown_table_length 26

/*
 *	expression_create()
 *
 *	Create a polynomial with only a constant and no unknowns.
 *
 *	@numerator: the numerator of the constant
 *	@denominator: the denominator of the constant
 */
exp expression_create(int numerator, int denominator) {
	exp ret;
	/*  Generate a new expression with no unknown  */
	ret.count = 0;
	ret.stack = empty_block_memory(DEFAULT_PAGE_SIZE);
	ret.cst = fraction_create(numerator, denominator);
	return(ret);
}

/*
 *	fraction_to_expression()
 *
 *	Convert a fraction to a polynomial.
 *
 *	@src: the fraction
 */
exp fraction_to_expression(fact src) {
	/*  Call expression_create function to create a new expression  */
	return(expression_create(src.denominator, src.denominator));
}

/*
 *	free_expression()
 *
 *	Free the storage pool of a polynomial.
 *
 *	@src: the fraction
 */
void free_expression(exp *ptr) {
	/*  Free the stack  */
	free_block_memory(&(ptr->stack));
	/*  AC the counter  */
	ptr->count = 0;
}

/*
 *	free_expression_stack()
 *
 *	Free the dynamic memory used by a polynomial.
 *
 *	@src: the fraction
 */
void free_expression_stack(exp *src, int count) {
	exp *ptr;
	/*  Free each node  */
	for (ptr = src; ptr < src + count; ptr++)
		free_expression(ptr);
	/*  Free the expression stack  */
	free(src);
}

/*
 *	query_expression_node()
 *
 *	Queries the specified item and stored in the pointer.
 *
 *	@stack: the unknown list
 *	@count: the counter
 *	@flag: specified item to be queried
 */
expnode* query_expression_node(bmem stack, int count, int flag) {
	return(query_expression_node_ex((expnode*)stack.ptr, ((expnode*)stack.ptr) + count - 1, flag));
}

/*
 *	query_expression_node_ex()
 *
 *	Queries the specified item and stored in the pointer.
 *
 *	@begin: the pointer points to the head of the unknown list
 *	@end: the pointer points to the end of the unknown list
 *	@flag: specified item to be queried
 */
expnode* query_expression_node_ex(expnode *begin, expnode *end, int flag) {
	expnode *ptr;
	/*  Check input values  */
	if (!begin || !end)
		return(NULL);
	if (end - begin <= 1) {
		/*  Only one or two node(s) in the table, just check it or them.  */
		if (begin->flag == flag)
			return(begin);
		if (end->flag == flag)
			return(end);
		return(NULL);
	} else {
		/*  Search the node in the table  */
		ptr = begin + (end - begin) / 2;
		if (flag == ptr->flag) {
			return(ptr);
		} else if (flag < ptr->flag) {
			return(query_expression_node_ex(begin, ptr - 1, flag));
		} else {
			return(query_expression_node_ex(ptr + 1, end, flag));
		}
	}
}

/*
 *	get_insert_position()
 *
 *	Get the position where the new node should be located in.
 *
 *	@p: the pointer points to the head of the unknown list
 *	@count: the counter
 *	@flag: specified item to be added into the list
 */
expnode* get_insert_position(expnode *p, int count, int flag) {
	expnode *ptr;
	if (!p)
		return(p);
	/*  Get the position where the new node should be located in (the node to be inserted mustn't be existed in the table)  */
	if (flag < p->flag)
		return(p);
	for (ptr = p; ptr < p + count - 1; ptr++)
		if (flag > ptr->flag && flag < (ptr + 1)->flag)
			return(ptr + 1);
	if (flag == ptr->flag)
		return(ptr);
	return(p + count);
}

/*
 *	push_expression_node()
 *
 *	Add a new node to the unknown list.
 *
 *	@target: the pointer points to the polynomial
 *	@nd: the new node
 *	@op: do this operation after deciding the position where it will be located in.
 */
bool push_expression_node(exp *target, expnode nd, factop op) {
	expnode *ptr, *move;
	int offset;
	/*  Check whether the unknown was existed  */
	ptr = query_expression_node(target->stack, target->count, nd.flag);
	if (ptr) {
		ptr->pfx = op(ptr->pfx, nd.pfx);
	} else {
		/*  Get the offset where the new node should be located in  */
		offset = get_insert_position((expnode*)target->stack.ptr, target->count, nd.flag) - (expnode*)target->stack.ptr;

		/*  Reallocate memory spaces  */
		if (!reallocate_block_memory(&(target->stack), (target->count + 1) * sizeof(expnode)))
			return(false);

		/*  Add the new node  */
		for (move = (expnode*)target->stack.ptr + target->count; move > (expnode*)target->stack.ptr + offset; move--)
			*move = *(move - 1);
		nd.pfx = op(F_ZERO, nd.pfx);
		*((expnode*)target->stack.ptr + offset) = nd;
		target->count++;
	}
	return(true);
}

/*
 *	push_expression_node_ex()
 *
 *	Add a new node to the unknown list.
 *
 *	@target: the pointer points to the polynomial
 *	@{flag, pfx}: the new node
 *	@op: do this operation after deciding the position where it will be located in.
 */
bool push_expression_node_ex(exp *target, int flag, fact pfx, factop op) {
	expnode nd;
	/*  Generate a new node  */
	nd.flag = flag;
	nd.pfx = pfx;
	return(push_expression_node(target, nd, op));
}

/*
 *	push_expression_constant()
 *
 *	Operate the constant.
 *
 *	@target: the pointer points to the polynomial
 *	@src: the fraction
 *	@op: do this operation
 */
void push_expression_constant(exp *target, fact src, factop op) {
	target->cst = op(target->cst, src);
}

/*
 *	expression_vf_basic()
 *
 *	Operate each node of the polynomial.
 *
 *	@target: the pointer points to the polynomial
 *	@src: the fraction
 *	@op: do this operation
 */
void expression_vf_basic(exp *target, fact src, factop op) {
	expnode *ptr;
	push_expression_constant(target, src, op);
	for (ptr = (expnode*)target->stack.ptr; ptr < (expnode*)target->stack.ptr + target->count; ptr++)
		ptr->pfx = op(ptr->pfx, src);
}

/*
 *	expression_plus()
 *
 *	Do plus between a polynomial and a fraction.
 *
 *	@target: the pointer points to the polynomial
 *	@src: the fraction
 *	@op: do this operation
 */
void expression_plus(exp *target, fact src) {
	push_expression_constant(target, src, fraction_plus);
}

/*
 *	expression_minus()
 *
 *	Do subtraction between a polynomial and a fraction.
 *
 *	@target: the pointer points to the polynomial
 *	@src: the fraction
 *	@op: do this operation
 */
void expression_minus(exp *target, fact src) {
	push_expression_constant(target, src, fraction_minus);
}

/*
 *	expression_multiplination()
 *
 *	Do multiplination between a polynomial and a fraction.
 *
 *	@target: the pointer points to the polynomial
 *	@src: the fraction
 *	@op: do this operation
 */
void expression_multiplination(exp *target, fact src) {
	expression_vf_basic(target, src, fraction_multiplination);
}

/*
 *	expression_division()
 *
 *	Do division between a polynomial and a fraction.
 *
 *	@target: the pointer points to the polynomial
 *	@src: the fraction
 *	@op: do this operation
 */
void expression_division(exp *target, fact src) {
	expression_vf_basic(target, src, fraction_division);
}

/*
 *	simplify_expression_node()
 *
 *	Remove unknown(s) with the prefix 0 before from the polynomial.
 *
 *	@target: the pointer points to the polynomial
 */
bool simplify_expression_node(exp *target) {
	int cnt = 0;
	bmem newstack;
	expnode *ptr, *tgptr;

	/*  Pre-calculate the size which needs to be allocated  */
	for (ptr = (expnode*)target->stack.ptr; ptr < (expnode*)target->stack.ptr + target->count; ptr++)
		if (fraction_compare(ptr->pfx, F_ZERO) != 0)
			cnt++;

	if (cnt == target->count)
		return(true);

	/*  Copy the data  */
	if (!cnt) {
		free_expression(target);
	} else {
		if (!allocate_block_memory(&newstack, cnt * sizeof(expnode), DEFAULT_PAGE_SIZE))
			return(false);
		for (ptr = (expnode*)target->stack.ptr, tgptr = (expnode*)newstack.ptr; ptr < (expnode*)target->stack.ptr + target->count; ptr++)
			if (fraction_compare(ptr->pfx, F_ZERO) != 0)
				*(tgptr++) = *ptr;
		free_block_memory(&(target->stack));
		target->stack = newstack;
		target->count = cnt;
	}
	return(true);
}

/*
 *	expression_double_operation()
 *
 *	Do operation: @exp1 = @exp1 @op1 (@exp2 @op2 @ft)
 */
bool expression_double_operation(exp *exp1, factop op1, exp exp2, factop op2, fact ft) {
	expnode *ptr;
	/*  op1 = {fraction_plus, fraction_minus), op2 = {fraction_multiplination, fraction_division}  */
	push_expression_constant(exp1, op2(exp2.cst, ft), op1);
	for (ptr = (expnode*)exp2.stack.ptr; ptr < (expnode*)exp2.stack.ptr + exp2.count; ptr++)
		if (!push_expression_node_ex(exp1, ptr->flag, op2(ptr->pfx, ft), op1))
			return(false);
	return(true);
}

/*
 *	finishing_expression_stack()
 *
 *	Finishing the set of polynomials and make their prefix number consecutively
 *
 *	@target: the pointer points to the polynomial set
 *	@len: the item count of the polynomial set
 */
void finishing_expression_stack(exp *src, int len) {
	expnode **fsort, **sp;
	exp *ptr;
	int min_flag, sorted;
	bool mf;

	fsort = (expnode**) malloc(len * sizeof(expnode*));
	if (!fsort)
		return;

	/*  Initialize  */
	for (ptr = src, sp = fsort; ptr < src + len; ptr++, sp++)
		*sp = (expnode*) ptr->stack.ptr;

	sorted = 0;

	/*  Sort them and make their ID consecutively  */
	do {
		for (mf = false, ptr = src, sp = fsort; ptr < src + len; ptr++, sp++)
			if (*sp < (expnode*)ptr->stack.ptr + ptr->count) {
				if (mf == false) {
					mf = true;
					min_flag = (*sp)->flag;
				} else {
					if ((*sp)->flag < min_flag)
						min_flag = (*sp)->flag;
				}
			}
		if (mf == true)
			for (sorted++, ptr = src, sp = fsort; ptr < src + len; ptr++, sp++)
				if (*sp < (expnode*)ptr->stack.ptr + ptr->count)
					if ((*sp)->flag == min_flag) {
						(*sp)->flag = sorted;
						(*sp)++;
					}
	} while(mf == true);

	free(fsort);
}

/*
 *	expression_memcpy()
 *
 *	Copy a polynomial from @src to @target
 *
 *	@...
 *	@destroy_after_copy: destroy 'src' after copying it
 */
bool expression_memcpy(exp *target, exp src, bool destroy_after_copy) {
	free_expression(target);
	*target = src;
	if (src.count) {
		if (!allocate_block_memory(&(target->stack), src.count * sizeof(expnode), DEFAULT_PAGE_SIZE))
			return(false);
		memcpy(target->stack.ptr, src.stack.ptr, src.count * sizeof(expnode));
	}
	if (destroy_after_copy)
		free_expression(&src);
	return(true);
}

/*
 *	expression_bce_setx2one()
 *
 *	Set the value last unknown to 1 (This function is useless for application but bce).
 *
 *	@src: the pointer points to the polynomial set
 *	@len: the item count of the polynomial set
 */
void expression_bce_setx2one(exp *src, int len) {
	exp *ptr;
	int maxid = EXPMODULE_BASEID;

	for (ptr = src; ptr < src + len; ptr++)
		if (ptr->count > 0)
			if (((expnode*)(ptr->stack.ptr) + ptr->count - 1)->flag > maxid)
				maxid = ((expnode*)(ptr->stack.ptr) + ptr->count - 1)->flag;

	if (maxid > EXPMODULE_BASEID)
		for (ptr = src; ptr < src + len; ptr++) {
			if (ptr->count > 0) {
				if (((expnode*)(ptr->stack.ptr) + ptr->count - 1)->flag == maxid) {
					ptr->cst = fraction_plus(ptr->cst, ((expnode*)(ptr->stack.ptr) + ptr->count - 1)->pfx);
					if (ptr->count == 1) {
						free_block_memory(&(ptr->stack));
					}
					(ptr->count)--;
				}
			}
		}
}

/*
 *	expression_to_number()
 *
 *	Make various elements of a polynomial into integers.
 *
 *	@src: the polynomial set
 *	@len: the item count of the polynomial set
 *	@inc_args: does it include the prefix numbers?
 */
void expression_to_number(exp *src, int len, bool inc_args) {
	exp *ptr;
	expnode *subptr;
	fact mt;
	int ml = 1;
	for (ptr = src; ptr < src + len; ptr++) {
		ml = mmul(ptr->cst.denominator, ml);
		if (inc_args)
			for (subptr = (expnode*)(ptr->stack.ptr); subptr < (expnode*)(ptr->stack.ptr) + ptr->count; subptr++)
				ml = mmul(subptr->pfx.denominator, ml);
	}
	mt = fraction_create((ml < 0) ? -ml : ml, 1);
	for (ptr = src; ptr < src + len; ptr++)
		expression_multiplination(ptr, mt);
}

/*
 *	get_unknown_symbol()
 *
 *	Get the symbol by using its id.
 *
 *	@id: the unknown's id
 */
char *get_unknown_symbol(int id) {
	int temp = id, cnt = 0;
	char *ret, *ptr;
	id--;
	temp--;
	do {
		cnt++;
		temp /= unknown_table_length;
	} while (temp != 0);
	ret = (char*) malloc((cnt + 1) * sizeof(char));
	if (!ret)
		return(NULL);
	ptr = ret + cnt - 1;
	*(ret + cnt) = '\0';
	do {
		*(ptr--) = unknown_table[id % unknown_table_length];
		id /= unknown_table_length;
	} while (id != 0);
	return(ret);
}

/*
 *	sprint_expression()
 *
 *	To convert a polynomial into a string.
 *
 *	@sz: the polynomial
 */
char* sprint_expression(exp sz) {
	char *new, *temp, *symbol;
	expnode *szptr;
	fact pfx;

	/*  Build a new string  */
	new = (char*) malloc(sizeof(char));
	if (!new)
		return(NULL);
	*new = '\0';

	if (sz.count) {
		for (szptr = (expnode*)(sz.stack.ptr); szptr < (expnode*)(sz.stack.ptr) + sz.count; szptr++) {
			pfx = szptr->pfx;
			symbol = get_unknown_symbol(szptr->flag);
			if (!symbol) {
				free(new);
				return(NULL);
			}
			temp = new;
			if (pfx.numerator % pfx.denominator == 0) {
				if (pfx.numerator / pfx.denominator == 1) {
					kasprintf(&new, "%sX%s+", new, symbol);
				} else {
					if (pfx.numerator / pfx.denominator != 0 && pfx.numerator / pfx.denominator != -1) {
						if (strlen(new) > 0 && pfx.numerator / pfx.denominator < 0)
							if (*(new + strlen(new) - 1) == '+')
								*(new + strlen(new) - 1) = '\0';
						kasprintf(&new, "%s%dX%s+", new, pfx.numerator / pfx.denominator, symbol);
					} else {
						kasprintf(&new, "%s-X%s+", new, symbol);
					}
				}
			} else {
				kasprintf(&new, "%s(%d/%d)X%s+", new, pfx.numerator, pfx.denominator, symbol);
			}
			free(symbol);
			if (new != temp)
				free(temp);
			if (!new)
				return(NULL);
		}
	}
	/*  Print out the constant  */
	pfx = sz.cst;
	temp = new;
	if (fraction_compare(pfx, F_ZERO) < 0) {
		if (pfx.numerator % pfx.denominator == 0)
			kasprintf(&new, "%s%d", new, pfx.numerator / pfx.denominator);
		else
			kasprintf(&new, "%s%d/%d", new, pfx.numerator, pfx.denominator);
	} else if (fraction_compare(pfx, F_ZERO) > 0) {
		if (strlen(new) > 0)
			*(new + strlen(new) - 1) = '\0';
		if (pfx.numerator % pfx.denominator == 0) {
			kasprintf(&new, "%s%d", new, pfx.numerator / pfx.denominator);
		} else {
			pfx.numerator = pfx.numerator > 0 ? -pfx.numerator : pfx.numerator;
			pfx.denominator = pfx.denominator < 0 ? -pfx.denominator : pfx.denominator;
			kasprintf(&new, "%s%d/%d", new, pfx.numerator, pfx.denominator);
		}
	} else {
		if (strlen(new) > 0)
			*(new + strlen(new) - 1) = '\0';
	}
	if (new != temp)
		free(temp);
	return(new);
}

