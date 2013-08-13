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


#ifndef _BLOCKMEM_H_
	#include "include/bool.h"
//typedef unsigned int size_t;
	struct block_memory_node {
		void *ptr;
		long int pnum;
		size_t psize;
	};
	typedef struct block_memory_node bmem;

	bmem empty_block_memory(size_t size_per_page);
	bool allocate_block_memory(bmem *p, size_t size, size_t size_per_page);
	bool reallocate_block_memory(bmem *p, size_t size);
	void free_block_memory(bmem *p);

	/*  Load FMA configurations  */
	#include "include/blockconf.h"

	#define _BLOCKMEM_H_
#endif



