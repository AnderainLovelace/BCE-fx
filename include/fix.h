/*
 *	Intelligent Chemical Equation Formatter/Fixer (ifix)
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

#ifndef _FIX_H_
	#include "include/polynomial.h"

	struct fixdata_adt {
		char *begin, *end;
		int side;
		exp num;
	};

	typedef struct fixdata_adt fix;

	#define FIX_SUCCESS 0
	#define FIX_FAILED 1

	#define JMP_ONE "1"

	int fix_compare(const void *p1, const void *p2);
	fix* fix_syntax_ce(char *nptr, exp *s, int count, int *fc);
	char* fix_redirect_sprint(fix *p, int fc, int exchangeable);

	#define _FIX_H_
#endif

