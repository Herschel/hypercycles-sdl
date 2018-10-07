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
#ifndef PORT_TYPES_H
#define PORT_TYPES_H

#include "Game.h"
#include <stdio.h>

// MIKE: Define away various obsolete modifiers.
#define _far
#define _interrupt
#define far
#define near

#ifdef __APPLE__
#define O_BINARY 0
#endif

extern Game game;
extern unsigned int *vga_ram;
extern unsigned char *vga_ram_c;
extern unsigned char red[257], green[257], blue[257];

extern void New_Key_Int( void );
extern int sdl_key;

void _enable( void );

void _disable( void );

int _dos_setvect( int i, void* handler );

void* _dos_getvect( int i );

#ifdef __APPLE__
char* itoa(int, char*, int);
char* strupr(char*);
#endif

long filelen( FILE* );
int getch( void );
int kbhit( void );
int inp(int);
void outp(int, int);

// TODO: Hoist this all out into the main loop.
// Will have to tear apart the mcp1 function...
void delay( int ms );
const char* getcmd( void );
int cprintf( const char * format, ... );

#endif // PORT_TYPES_H
