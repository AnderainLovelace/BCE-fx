/*
 *	Fast Memory Re-allocate(FMA) System for Hign Performance Computing
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
#include "../include/bool.h"
#include "../include/blockmem.h"

/*
 *	empty_block_memory()
 *
 *	Create a empty memory page
 *
 *	@size_per_page: the size of each page
 */
bmem empty_block_memory(size_t size_per_page) {
	bmem new;
	new.pnum = 0;
	new.psize = size_per_page;
	new.ptr = NULL;
	return(new);
}

/*
 *	allocate_block_memory()
 *
 *	Allocate dynamic memory
 *
 *	@p: the pointer points to the data struct of the container
 *	@size: numbers of bytes to allocate
 *	@size_per_page: the size of each page
 */
bool allocate_block_memory(bmem *p, size_t size, size_t size_per_page) {
	/*  Calculate the number of pages what we need  */
	p->psize = size_per_page;
	p->pnum = size / size_per_page + 1;
	/*  Allocate memory spaces  */
	p->ptr = malloc(p->pnum * p->psize);
	if (!p->ptr)
		return(false);
	return(true);
}

/*
 *	reallocate_block_memory()
 *
 *	Changes the size of the memory block pointed to
 *
 *	@p: the pointer points to the data struct of the container
 *	@size: numbers of bytes the new block will be
 */
bool reallocate_block_memory(bmem *p, size_t size) {
	bmem new;
	/*  The block which is uninitialized can't be reallocated  */
	if (!p->psize)
		return(false);
	/*  Calculate the number of pages what we need  */
	new.psize = p->psize;
	new.pnum = size / new.psize + 1;
	if (new.pnum != p->pnum) {
		/*  If we need new pages or we don't need so many pages, reallocate the memory spaces  */
		/*  PS: I call this step 'change page'  */
		new.ptr = realloc(p->ptr, new.psize * new.pnum);
		if (!new.ptr)
			return(false);
	} else {
		new.ptr = p->ptr;
	}
	*p = new;
	return(true);
}

/*
 *	free_block_memory()
 *
 *	Free the memory space pointed to by 'p'.
 *
 *	@p: the pointer points to the data struct of the container
 */
void free_block_memory(bmem *p) {
	if (p->ptr);
		free(p->ptr);
	p->ptr = NULL;
	*p = empty_block_memory(p->psize);
}

