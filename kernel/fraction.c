/*
 *	Fraction Computing Module
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
#include "../include/mmdiv.h"
#include "../include/fraction.h"

/*
 *	fraction_create()
 *
 *	Create a fraction
 *
 *	@numer: the numerator part of the fraction
 *	@denomin: the denominator part of the fraction
 */
fact fraction_create(int numer, int denomin) {
	fact new_fact;

	new_fact.numerator = numer;
	new_fact.denominator = (numer != 0 ? denomin : 1);

	new_fact = fraction_simplify(new_fact);

	return(new_fact);
}

/*
 *	fraction_simplify()
 *
 *	Simplify a fraction
 *
 *	@src: the fraction
 */
fact fraction_simplify(fact src) {
	int ld_mdiv = mdiv(src.numerator, src.denominator);
	fact ret;

	ret.numerator = src.numerator / ld_mdiv;
	ret.denominator = src.denominator / ld_mdiv;

	fraction_check_zero(ret);

	return(ret);
}

/*
 *	fraction_plus()
 *
 *	Do plus between @num1 and @num2
 */
fact fraction_plus(fact num1, fact num2) {
	fact ret;

	fraction_check_zero(num1);
	fraction_check_zero(num2);

	ret = fraction_create(num1.numerator * num2.denominator + num2.numerator * num1.denominator, num1.denominator * num2.denominator);
	ret = fraction_simplify(ret);

	return(ret);
}

/*
 *	fraction_plus()
 *
 *	Do subtract between @num1 and @num2
 */
fact fraction_minus(fact num1, fact num2) {
	fact ret;

	fraction_check_zero(num1);
	fraction_check_zero(num2);

	ret = fraction_create(num1.numerator * num2.denominator - num2.numerator * num1.denominator, num1.denominator * num2.denominator);
	ret = fraction_simplify(ret);

	return(ret);
}

/*
 *	fraction_plus()
 *
 *	Do multiplination between @num1 and @num2
 */
fact fraction_multiplination(fact num1, fact num2) {
	fact ret;

	fraction_check_zero(num1);
	fraction_check_zero(num2);

	ret = fraction_create(num1.numerator * num2.numerator, num1.denominator * num2.denominator);
	ret = fraction_simplify(ret);

	return(ret);
}

/*
 *	fraction_plus()
 *
 *	Do division between @num1 and @num2
 */
fact fraction_division(fact num1, fact num2) {
	fact ret;

	fraction_check_zero(num1);
	fraction_check_zero(num2);

	ret = fraction_create(num1.numerator * num2.denominator, num1.denominator * num2.numerator);
	ret = fraction_simplify(ret);

	return(ret);
}

/*
 *	fraction_plus()
 *
 *	Compare the value of two fractions(@x, @y)
 *	fraction_compare(x,y)<0 => x>y
 *	                (x,y)=0 => x=y
 *                     (x,y)>0 => x<y
 */
int fraction_compare(fact x, fact y) {
	int ln_mmul;

	x = fraction_simplify(x);
	y = fraction_simplify(y);

	ln_mmul = mmul(x.denominator, y.denominator);

	return(y.numerator * ln_mmul / y.denominator - x.numerator * ln_mmul / x.denominator);
}

