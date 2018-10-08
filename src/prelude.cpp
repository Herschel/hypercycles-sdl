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
#include "prelude.h"
#include <stdio.h>
#include "SDL.h"

int sdl_key;

#if PLATFORM != win64

char* itoa(int value, char* str, int base)
{
    sprintf(str, "%d", value);
	return str;
}

char* strupr(char* s)
{
    while(*s != 0)
    {
        *s = toupper(*s);
        s++;
    }
    return s;
}
#endif

long filelen( FILE* fd )
{
	long len = 0;
	if( fd != NULL && fseek( fd, 0, SEEK_END ) == 0 )
	{
		len = ftell( fd );
		fseek( fd, 0, SEEK_SET );
	}
	return len;
}

int getch( void )
{
	return 0;
}

int inp( int a )
{
	return 0;
}

void outp(int a, int b)
{
}

int kbhit( void )
{
	return 0;
}

// TODO: Hoist this all out into the main loop.
// Will have to tear apart the mcp1 function...
void delay(int ms)
{
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = 320;
	r.h = 200;


	unsigned int* pixels;
	int pitch;
	SDL_LockTexture(game.screen, &r, (void**)&pixels, &pitch);
	for (int i = 0; i < 320 * 200; i++)
	{
		unsigned char pal = vga_ram_c[i];
		unsigned int r = red[ pal ] << 2;
		unsigned int g = green[pal] << 2;
		unsigned int b = blue[pal] << 2;
		pixels[i] = (r << 24) | (g << 16) | (b << 8) | 0xff;
	}
	SDL_UnlockTexture(game.screen);
	
	SDL_RenderCopy(game.renderer, game.screen, NULL, NULL);
	SDL_RenderPresent(game.renderer);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		/* handle your event here */
		switch (event.type)
		{

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				sdl_key = 1;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				sdl_key = 72;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				sdl_key = 80;
			}
			else if (event.key.keysym.sym == SDLK_LEFT)
			{
				sdl_key = 75;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				sdl_key = 77;
			}
			else if (event.key.keysym.sym == SDLK_RETURN)
			{
				sdl_key = 28;
			}
			else
			{
				sdl_key = (int)event.key.keysym.sym;
			}
			New_Key_Int();
			break;

		case SDL_QUIT:
			exit(0);
			break;
		}
	}


	SDL_Delay( ms );
}

const char* getcmd()
{
	printf("getcmd\n");
	return "";
}

int cprintf( const char * format, ... )
{
	return 0;
}

