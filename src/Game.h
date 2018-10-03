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

#ifndef HC_GAME_H
#define HC_GAME_H

#include <stdbool.h>
#include "SDL.h"
#include "SDL_mixer.h"

typedef struct {
	SDL_Window*		window;
	SDL_Renderer*   renderer;
	SDL_Texture*    screen;	
} Game;

bool Game_Init(Game* game);
void Game_Destroy( Game* game );

#endif // HC_GAME_H
