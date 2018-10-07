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

#include "Game.h"
#include <stdio.h>

bool Game_Init( Game* game )
{
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return false;
	}

	if( SDL_CreateWindowAndRenderer(320, 200, SDL_WINDOW_ALLOW_HIGHDPI, &game->window, &game->renderer) )
	{
		printf("SDL_CreateWindowAndRenderer Error: %s\n", SDL_GetError());
		return false;
	}

	game->screen = SDL_CreateTexture( game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 320, 200 );
	if( game->screen == NULL )
	{
		printf("SDL_CreateTexture Error: %s", SDL_GetError());
		return false;
	}

	if (Mix_OpenAudio(11025, AUDIO_U8, 1, 1024) == -1)
	{
		printf("Mix_OpenAudio failed");
		return false;
	}

	if (Mix_AllocateChannels(16) != 16)
	{
		printf("Mix_AllocateChannels failed");
		return false;
	}
	
	return true;
}

void Game_Destroy( Game* game )
{
	Mix_CloseAudio();

	SDL_DestroyRenderer( game->renderer );
	SDL_DestroyWindow( game->window );
	SDL_Quit();

	game->renderer = NULL;
	game->window = NULL;
	game->screen = NULL;
}