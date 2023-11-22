#include <SDL2/SDL.h>
#include "types.h"
#include "snake.h"
#include "renderer.h"

#ifndef __GAME_H
#define __GAME_H

#define GAME_SPEED 10

// Grid of GAME_WIDTH x GAME_HEIGH
#define GAME_WIDTH 76
#define GAME_HEIGHT 64

#define BORDER_OFFSET 2

#define CELL_WIDTH (WINDOW_WIDTH / GAME_WIDTH)
#define CELL_HEIGHT (WINDOW_HEIGHT / GAME_HEIGHT)

#define APPLE_IMG_PATH "assets/apple.png"

typedef struct Fruit
{
    int x;
    int y;
} Fruit;

typedef struct Game
{
    bool isRunning;
    bool isGameOver;
    int speed;
    int score;
    int fruitWidth;
    int fruitHeigh;
    Fruit *fruit;
    Snake *snake;
    SDL_Texture *fruitTexture;
} Game;

Game *Game_Init();
Game *Game_Get();
void Game_Start();
void Game_Stop();
void Game_Update();
void Game_Render();
void Game_EatFruit();
void Game_SpawnFruit();
void Game_Destroy();

#endif // __GAME_H