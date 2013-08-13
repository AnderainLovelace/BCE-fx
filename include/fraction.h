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

#ifndef _FRACTION_H_
	struct fractionStruct
	{
		int denominator;
		int numerator;
	};
	typedef struct fractionStruct fact;
	typedef fact (*factop)(fact, fact);

	fact fraction_create(int numer, int denomin);
	fact fraction_simplify(fact src);
	fact fraction_plus(fact num1, fact num2);
	fact fraction_minus(fact num1, fact num2);
	fact fraction_multiplination(fact num1, fact num2);
	fact fraction_division(fact num1, fact num2);
	int fraction_compare(fact x, fact y);

	#define F_ZERO fraction_create(0, 1)
	#define fraction_check_zero(x)	\
		if (x.numerator == 0)	\
			x.denominator = 1;

	#define _FRACTION_H_
#endif


