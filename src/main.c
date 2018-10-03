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
#include <stdio.h>
#include "SDL.h"
#include "SDL_mixer.h"

extern void main_hyper6(void);

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

int main( int argc, char* argv[] )
{
    if( SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
        printf( "SDL_Init Error: %s\n", SDL_GetError() );
        return 1;
    }

	if( SDL_CreateWindowAndRenderer(320, 200, SDL_WINDOW_ALLOW_HIGHDPI, &window, &renderer) )
	{
		printf("SDL_CreateWindowAndRenderer Error: %s\n", SDL_GetError());
		return 1;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 320, 200);
	if( texture == NULL )
	{
		printf("SDL_CreateTexture Error: %s", SDL_GetError());
		return 1;
	}

	main_hyper6();

	SDL_DestroyTexture( texture );
	SDL_DestroyWindow( window );

    SDL_Quit();
    return 0;
}
