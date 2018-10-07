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
// MIKE TODO: Implement these.

int fixmul(int a, int b)
{
	// 16.16?
	return  ((long long)a * (long long)b) >> 16;
}

int fixmul2(int a, int b)
{
	// 8.24?
	return  ((long long)a * (long long)b) >> 24;
}

int fixdiv(int a, int b)
{
	return ((long long)a * (1 << 16)) / b;
}

int shf1(int a)
{
	return a >> 22;
}

int qrshift(int a, int b)
{
	return a >> b;
}
