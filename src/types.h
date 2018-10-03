/*
* hypercycles-sdl (https://github.com/Herschel/hypercycles-sdl)
* Copyright (c) 2015 The hypercycles-sdl authors
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//-------+---------+---------+---------+---------+---------+---------+---------+
// 1991-1992 Betz Associates. Released to the Public Domain. This source code
// is offered without warranty or guarantee of any kind, and the author(s)
// expressly disclaim any responsibility for consequences of it's use in
// products for commercial, shareware, or public domain release of any kind.
//
// Release of copyright applies only to this source code module, and does
// not apply to any other source code which may be included with it in a
// program
//
// File Name: TYPES.H
// Project:   Flights of Fantasy
// Creation:  August 2, 1992
// Author:    Mark Betz (MB)
//
// Machine:   IBM PC and Compatibles
//
// Includes:  nothing
//
//-------+---------+---------+---------+---------+---------+---------+---------+
// Change History
// ------ -------
//
//      Date            Rev.    Author          Purpose
//      ----            ----    ------      -------
//      8-2-1992        1.0     MB          Initial development
//      8-29-1992       1.1b    MB          first beta
//      9-26-1992       1.1     MB          publication release
//
// Description
// -----------
//      Provides some basic redefinition of native C types
//-------+---------+---------+---------+---------+---------+---------+---------+

#ifndef __TYPES__
#define __TYPES__

typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
typedef int boolean;

#define int false = 0;
#define int true = 1;

#endif
