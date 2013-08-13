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

#ifndef _SYNTAX_H
	#include "include/fraction.h"
	#include "include/polynomial.h"

	#define SYNTAX_EQUAL '='
	#define SYNTAX_SPLITER "+=;"

	#define connector_plus "+"
	#define connector_equal "="
	#define connector_opposite_equal "<=>"
	#define connector_null ""
	#define connector_nullrside "=0"

	fact** get_balance_matrix(char *nptr, int *high_x, int *high_y);

	#define _SYNTAX_H_
#endif

