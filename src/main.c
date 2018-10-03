#include <stdio.h>
#include "SDL.h"

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
