/*
 *	Extra String Library
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
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "../include/bool.h"
#include "../include/string_ext.h"

/*
 *	strpush()
 *
 *	Split the string between [@begin, @end].
 */

char* strpush(const char *begin, const char *end) {
	char *ret;
	ret = (char*) malloc((end - begin + 2)*sizeof(char));
	if (!ret)
		return(NULL);

	*(ret + strplen(begin, end)) = '\0';
	memcpy(ret, begin, strplen(begin, end) * sizeof(char));

	return(ret);
}

/*
 *	strpcomp()
 *
 *	Compare a string with the string between [@begin, @end].
 */
bool strpcomp(const char *begin, const char *end, const char *data) {
	return(strpqcomp(begin, end, data, data + strlen(data) - 1));
}

/*
 *	strpqcomp()
 *
 *	Compare the string [@begin1, @end1] with the string between [@begin2, @end2].
 */
bool strpqcomp(const char *begin1, const char *end1, const char *begin2, const char *end2) {
	for (; *begin1 == *begin2; begin1++, begin2++)
		if (begin1 == end1 || begin2 == end2) {
			if (begin1 != end1 || begin2 != end2)
				return(false);
			else
				return(true);
		}

	return(false);
}

/*
 *	strpisnum()
 *
 *	Determine whether the string between [@begin, @end] is a readable number.
 */
bool strpisnum(const char *begin, const char *end) {
	for (; begin <= end; begin++)
		if (isdigit((int)(*begin)))
			continue;
		else
			return(false);

	return(true);
}

/*
 *	strptoi()
 *
 *	Convert the string between [@begin, @end] to an integer.
 */
int strptoi(char *begin, char *end, char **endptr) {
	int ret = 0;

	if (endptr)
		*endptr = end + 1;

	for (; begin <= end; begin++)
		if (isdigit((int)(*begin))) {
			ret = ret * 10 + (*begin) - '0';
		} else {
			if (endptr)
				*endptr = begin;
			break;
		}

	return(ret);
}

