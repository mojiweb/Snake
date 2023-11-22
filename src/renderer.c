#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "renderer.h"
#include "log.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int Renderer_Init(char *windowCaption)
{
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        LogError("SDL_Init failed: %s", SDL_GetError());
        return 0;
    }
    
    window = SDL_CreateWindow(windowCaption,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,WINDOW_HEIGHT,
                              0);
    if(window == NULL){
        LogError("SDL_CreateWindow failed: %s",SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        LogError("SDL_CreateRenderer failed: %s",SDL_GetError());
        return 0;
    }

    SDL_Surface * icon = IMG_Load("assets/gfx/snake.png");

    if ( icon == NULL )
    {
        LogError("IMG_Load failed: %s",SDL_GetError());
        return 0;
    }
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    return 1;
}

SDL_Renderer * Renderer_GetRenderer()
{
    return renderer;
}

SDL_Window * Renderer_GetWindow()
{
    return window;
}

void Renderer_Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    // We safely uninitialize SDL2
    SDL_Quit();
}