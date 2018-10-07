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
// OUTCHIP.C
//       Set the register at address 'addr' to the value 'data'.
//       Take care of data register to data register write delay.

#include <stdlib.h>

extern unsigned int genAddr;

void SndOutput( int addr1,int data1)
{
	//// MIKE
 //int a,b,c;
 //c = genAddr;
 //outp(c,addr1);
 //
 //// 12 cyles delay ( 3.6 Mhz )
 //for(a=0;a<6;a++) b=inp(c);
 //
 //outp(c+1,data1);       
 //
 //// 84 cyles delay ( 3.6 Mhz )
 //for(a=0;a<35;a++) b=inp(c);
	//printf("SndOutput %d %d\n", addr1, data1);
}

