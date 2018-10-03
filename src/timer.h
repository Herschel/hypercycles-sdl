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
/* TIMER.H - Header file for TIMER.C                               */
/* By Jim Dos‚.  This program is released to the public domain and */
/* may be used in your own programs.  Leave me a note if you find  */
/* it helpful!                                                     */
/* On CompuServe: Jim Dose' 76346,2061                             */
/* On AOL:        JimDose                                          */

void Set8253Rate( int speed );
void SetClockRate( int TicksPerSecond );
void InitTimer( int TicksPerSecond );
void RestoreTimer( void );
unsigned long GetCounter( void );
void SetCounter( unsigned long count );
