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
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _EQUATION_H_
	#include "include/bool.h"
	#include "include/fraction.h"
	#include "include/polynomial.h"

	exp *solve_equations(fact **matrix, int mx, int my, int unknowns, int base_offset_x, int base_offset_y);
	bool check_equation_result(fact **matrix, exp *ret, int mx, int my);

	#define _EQUATION_H_
#endif

