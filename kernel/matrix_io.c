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

#include <stdio.h>
#include <stdlib.h>
#include "../include/fraction.h"
#include "../include/matrix_io.h"

/*
 *	get_matrix_header()
 *
 *	Get the first node of the memory pool of a matrix.
 *
 *	@matrix: the matrix
 *	@my: the number of rows of the matrix
 */
fact* get_matrix_header(fact **matrix, int my) {
	fact *min, **eptr = matrix + my;

	for (min = *matrix; matrix < eptr; matrix++)
		if (*matrix < min)
			min = *matrix;

	return(min);
}

/*
 *	free_matrix()
 *
 *	Destroy a matrix.
 *
 *	@matrix: the matrix
 *	@my: the number of rows of the matrix
 */
void free_matrix(fact **matrix, int my) {
	free(get_matrix_header(matrix, my));
	free(matrix);
}

/*
 *	allocate_matrix()
 *
 *	Allocate a matrix.
 *
 *	@x: the number of columns of the matrix
 *	@y: the number of rows of the matrix
 */
fact** allocate_matrix(int x, int y) {
	fact *area, **matrix, **ptr;

	area = (fact*) malloc(x * y * sizeof(fact));
	if (!area)
		return(NULL);

	matrix = (fact**) malloc(y * sizeof(fact*));
	if (!matrix) {
		free(area);
		return(NULL);
	}

	for (ptr = matrix; y > 0; area+=x, y--)
		*(ptr++) = area;

	return(matrix);
}

/*
 *	write_matrix()
 *
 *	Write data to a matrix.
 *
 *	@matrix: the matrix
 *	@offset_x: the offset column position
 *	@offset_y: the offset row position
 *	@data: the data
 */
void write_matrix(fact **matrix, int offset_x, int offset_y, fact data) {
	matrix[offset_y][offset_x] = data;
}

/*
 *	read_matrix()
 *
 *	Read data from a matrix.
 *
 *	@matrix: the matrix
 *	@offset_x: the offset column position
 *	@offset_y: the offset row position
 */
fact read_matrix(fact **matrix, int offset_x, int offset_y) {
	return(matrix[offset_y][offset_x]);
}

