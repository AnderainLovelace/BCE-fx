/*
 *	Molecule Analyzer
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

#ifndef _MOLECULE_H
	#include "include/bool.h"
	#include "include/element.h"
	#include "include/blockmem.h"

	#define ELECTRIC_E_PLUS "e+"
	#define ELECTRIC_E_MINUS "e-"
	#define ELECTRIC_E_NAME "e"

	#define HYDRATE_DOT '.'

	#define SPECIAL_SIGNATURE_SOLID "s"
	#define SPECIAL_SIGNATURE_LIQUIT "l"
	#define SPECIAL_SIGNATURE_GAS "g"

	bool solve_molecule(char *begin, char *end, bmem *eptr, int *ecount, int suffix);
	bool solve_molecule_hydrate(char *begin, char *end, bmem *eptr, int *ecount, int suffix);

	#define _MOLECULE_H
#endif

