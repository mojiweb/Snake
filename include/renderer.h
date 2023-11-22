#ifndef __RENDERER_H
#define __RENDERER_H

#define WINDOW_WIDTH     760
#define WINDOW_HEIGHT    640

int Renderer_Init(char *windowCaption);
SDL_Renderer * Renderer_GetRenderer();
SDL_Window * Renderer_GetWindow();
void Renderer_Destroy();

#endif // __RENDERER_H