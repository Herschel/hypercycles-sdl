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
#include "port_types.h"
#include <stdio.h>
#include "SDL.h"

void _enable()
{
	// MIKE TODO: Remove.
}

void _disable()
{
	// MIKE TODO: Remove.
}

int _dos_setvect(int i, void* handler)
{
	printf("_dos_setvect %d\n", i);
	return 0;
}

void* _dos_getvect(int i)
{
	printf("_dos_getvect %d\n", i);
	return NULL;
}

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* texture;
extern unsigned int *vga_ram;
extern unsigned char *vga_ram_c;
extern unsigned char red[257], green[257], blue[257];

extern void New_Key_Int(void);
int sdl_key;

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
	int* pitch;
	SDL_LockTexture(texture, &r, &pixels, &pitch);
	for (int i = 0; i < 320 * 200; i++)
	{
		unsigned char pal = vga_ram_c[i];
		unsigned int r = red[ pal ] << 2;
		unsigned int g = green[pal] << 2;
		unsigned int b = blue[pal] << 2;
		pixels[i] = (r << 24) | (g << 16) | (b << 8) | 0xff;
	}
	SDL_UnlockTexture(texture);
	
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

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
