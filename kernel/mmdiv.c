/*
 *	The Euclidean Algorithm Module (GCD)
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

#include "../include/mmdiv.h"

/*
 *	mdiv()
 *
 *	Get the greatest common divisor of two numbers.
 *
 *	@x, @y: the two numbers.
 */
int mdiv(int x, int y) {
	int temp;
	while (y) {
		temp = y;
		y = x % y;
		x = temp;
	}
	return(x);
}

/*
 *	mmul()
 *
 *	Get the least common multiple of two numbers.
 *
 *	@x, @y: the two numbers.
 */
int mmul(int x, int y) {
	return(x * y / mdiv(x, y));
}

