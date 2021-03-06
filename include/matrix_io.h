/*
 *	Matrix Manager
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

#ifndef _MATRIX_IO_H_
	/*  ADT type  */
	#include "include/fraction.h"

	fact *get_matrix_header(fact **matrix, int my);
	void free_matrix(fact **matrix, int my);
	fact **allocate_matrix(int x, int y);
	void write_matrix(fact **matrix, int offset_x, int offset_y, fact data);
	fact read_matrix(fact **matrix, int offset_x, int offset_y);

	#define matrix_offset(matrix, offset_x, offset_y) ((*((matrix) + (offset_y))) + (offset_x))

	#define _MATRIX_IO_H_
#endif

