#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL_ttf.h"
#include <stdlib.h>
#include "menu.h"
#include "game.h"
#include "renderer.h"
#include "log.h"

static Menu *menu;

void Menu_MenuItemMoveUp();
void Menu_MenuItemMoveDown();
int Menu_GetSelectedItem();
void Menu_SetSelectedItem(int selected);

void Menu_Init(SDL_Renderer *renderer)
{
    menu = (Menu *)malloc(sizeof(Menu));

    menu->renderer = renderer;

    // Init Start Game menu item
    strcpy(menu->itemList[START_MENU_ITEM].name, "New game");
    menu->itemList[START_MENU_ITEM].isSelected = true;

    // Init Quit Game menu item
    strcpy(menu->itemList[QUIT_MENU_ITEM].name, "Quit");
    menu->itemList[QUIT_MENU_ITEM].isSelected = false;

    menu->isShown = false;
}

int Menu_Show(Game *game)
{
    SDL_Surface *messageSurface;
    SDL_Texture *message;
    SDL_Rect Message_rect, Logo_rect;
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {224, 0, 0, 255};
    SDL_Color green = {0, 224, 0, 255};
    SDL_Color color = white;
    TTF_Font *font;
    int texW, texH, posY, selectedAction = -1;

    if (TTF_Init() < 0)
    {
        LogError("Error from TTF_Init: %s", TTF_GetError());
        return -1;
    }

    if (!(font = TTF_OpenFont("assets/fonts/PixelifySans-Regular.ttf", 60)))
    {
        LogError("Error from TTF_OpenFont: %s", TTF_GetError());
        return -1;
    }

    SDL_Texture *logo = IMG_LoadTexture(menu->renderer, "assets/gfx/logo2.png");
    if (logo == NULL)
    {
        LogError("IMG_Load failed: %s", SDL_GetError());
        return -1;
    }

    menu->isShown = true;

    while (menu->isShown)
    {
        // Clear the screen
        SDL_SetRenderDrawColor(menu->renderer, 32, 32, 32, 0);
        SDL_RenderClear(menu->renderer);

        posY = 50;

        if (game && game->isGameOver)
        {
            if (!(messageSurface = TTF_RenderText_Blended(font, "Game Over !!!", red)))
            {
                LogError("Error from TTF_RenderText_Blended: %s", TTF_GetError());
            }

            if (!(message = SDL_CreateTextureFromSurface(menu->renderer, messageSurface)))
            {
                LogError("Error from SDL_CreateTextureFromSurface: %s", SDL_GetError());
                LogError(SDL_GetError());
            }

            SDL_QueryTexture(message, NULL, NULL, &texW, &texH);

            Message_rect.x = ((WINDOW_WIDTH / 2) - (texW / 2));
            Message_rect.y = posY;
            Message_rect.w = texW;
            Message_rect.h = texH;

            posY += texH;

            SDL_RenderCopy(menu->renderer, message, NULL, &Message_rect);
        }
        else
        {
            SDL_QueryTexture(logo, NULL, NULL, &texW, &texH);

            Logo_rect.x = ((WINDOW_WIDTH / 2) - (texW / 2));
            Logo_rect.y = posY;
            Logo_rect.w = texW;
            Logo_rect.h = texH;

            posY += texH;

            SDL_RenderCopy(menu->renderer, logo, NULL, &Logo_rect);
        }

        posY += 50;

        // Render menu items (you can use SDL functions like SDL_RenderDrawRect, SDL_RenderCopy, etc.)
        for (int i = 0; i < MENU_ITEM_COUNT; i++)
        {
            color = (menu->itemList[i].isSelected) ? green : white;

            if (!(messageSurface = TTF_RenderText_Blended(font, menu->itemList[i].name, color)))
            {
                LogError("Error from TTF_RenderText_Blended: %s", TTF_GetError());
            }

            if (!(message = SDL_CreateTextureFromSurface(menu->renderer, messageSurface)))
            {
                LogError("Error from SDL_CreateTextureFromSurface: %s", SDL_GetError());
                LogError(SDL_GetError());
            }

            SDL_QueryTexture(message, NULL, NULL, &texW, &texH);

            Message_rect.x = ((WINDOW_WIDTH / 2) - (texW / 2));
            Message_rect.y = posY;
            Message_rect.w = texW;
            Message_rect.h = texH;

            posY += texH + 50;

            SDL_RenderCopy(menu->renderer, message, NULL, &Message_rect);
        }

        // Present the renderer
        SDL_RenderPresent(menu->renderer);

        // Handle user input to navigate the menu
        SDL_Event event;

        // Events management
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                // keyboard API for key pressed
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    selectedAction = -1;
                    menu->isShown = false;
                    break;
                case SDL_SCANCODE_SPACE:
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_KP_ENTER:
                    selectedAction = Menu_GetSelectedItem();
                    menu->isShown = false;
                    break;
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    Menu_MenuItemMoveUp();
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    Menu_MenuItemMoveDown();
                    break;
                default:
                    break;
                }
                break;
            }
        }
    }

    TTF_Quit();
    SDL_DestroyTexture(message);
    SDL_FreeSurface(messageSurface);

    return selectedAction;
}

int Menu_GetSelectedItem()
{
    for (int i = 0; i < MENU_ITEM_COUNT; i++)
    {
        if (menu->itemList[i].isSelected)
        {
            return i;
        }
    }

    return -1;
}

void Menu_SetSelectedItem(int selected)
{
    if (selected < 0 || selected >= MENU_ITEM_COUNT)
    {
        LogWarn("Menu_SetSelectedItem invalid selected index %d", selected);
        return;
    }

    for (int i = 0; i < MENU_ITEM_COUNT; i++)
    {
        menu->itemList[i].isSelected = false;
    }

    menu->itemList[selected].isSelected = true;
}

void Menu_MenuItemMoveUp()
{
    int selected = Menu_GetSelectedItem();

    if (selected > 0)
    {
        Menu_SetSelectedItem(selected - 1);
    }
}

void Menu_MenuItemMoveDown()
{
    int selected = Menu_GetSelectedItem();

    if (selected < (MENU_ITEM_COUNT - 1))
    {
        Menu_SetSelectedItem(selected + 1);
    }
}

void Menu_Destroy()
{
    if (menu != NULL)
        free(menu);
}