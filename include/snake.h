#ifndef __SNAKE_H
#define __SNAKE_H

#include <SDL2/SDL.h>
#include "types.h"

#define SNAKE_START_LENGTH 5
#define SNAKE_START_X 16
#define SNAKE_START_Y 16

typedef struct SnakeBody SnakeBody;
struct SnakeBody
{
    int x;
    int y;
    SnakeBody *next;
};

typedef struct Snake
{
    SnakeBody *tail;
    int xDirection;
    int yDirection;
} Snake;

#include "game.h"

Snake *Snake_Init();
Snake *Snake_Get();
void Snake_AddBodyElement();
void Snake_MoveLeft();
void Snake_MoveRight();
void Snake_MoveUp();
void Snake_MoveDown();
void Snake_Move(Game *game);
bool Snake_Collision();

#endif // __SNAKE_H