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
#define COS(X) fx_cos_table[X & 4095]
//(NUMBER_OF_DEGREES-1)]
#define SIN(X) fx_sin_table[X & 4095]

#define NUMBER_OF_DEGREES  4096
#define SHIFT  16
#define SHIFT_MULT  1<<SHIFT;

extern int fx_cos_table[4096];
extern int fx_sin_table[4096];
