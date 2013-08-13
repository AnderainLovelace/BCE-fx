/*
 *	Cross-platform 'kasprintf' Function
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
#include <stdarg.h>
#include <string.h>
#include "../include/kasprintf.h"

/*
 *	kasprintf()
 *
 *	Print to allocated string.
 */

char * d_strdup (const char * s)
{
	int len = strlen(s);
	char * p = calloc(len+1,1);
	return strcpy(p,s);
}

int kasprintf(char **buf,  const char *fmt, ...) {
	va_list ap;
	int len;
	char *p,b[512];

	va_start(ap, fmt);
	len = vsprintf(b, fmt, ap);
	va_end(ap);

	*buf = d_strdup(b);

	return(len);
}

