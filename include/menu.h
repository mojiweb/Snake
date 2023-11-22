#ifndef __MENU_H
#define __MENU_H

#include "types.h"
#include "game.h"

enum MENU_ITEM_LIST
{
    START_MENU_ITEM,
    QUIT_MENU_ITEM,
    MENU_ITEM_COUNT // Keep in the last position.
};

typedef struct MenuItem
{
    char name[32];
    bool isSelected;
} MenuItem;

typedef struct Menu
{
    SDL_Renderer *renderer;
    bool isShown;
    MenuItem itemList[MENU_ITEM_COUNT];
} Menu;

void Menu_Init(SDL_Renderer *renderer);
bool Menu_AddItem(MenuItem *item);
int Menu_Show(Game *game);
void Menu_HandleInput();
void Menu_Destroy();

#endif // __MENU_H
