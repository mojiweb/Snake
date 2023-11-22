#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "renderer.h"
#include <time.h>
#include "types.h"
#include "game.h"
#include "menu.h"

int main(int argc, char * argv[])
{
    bool quit = false;
    Game * game = NULL;
    srand(time(0));
    
    // Create Window and Renderer
    if ( Renderer_Init("Snake") == 1 )
    {
        Menu_Init(Renderer_GetRenderer());
        
        while ( ! quit )
        {
            switch (Menu_Show(game))
            {
                case START_MENU_ITEM:
                    if ( ! game || ( game && ! game->isRunning ) )
                    {
                        game = Game_Init();
                        Game_Start();
                    }
                    break;
                case QUIT_MENU_ITEM:
                    Game_Destroy();
                    quit = true;
                    break;
                default:
                    break;
            }
        }

        Menu_Destroy();
    }
    
    Renderer_Destroy();

    return 0;
}