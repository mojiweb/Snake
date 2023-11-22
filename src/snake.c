#include "game.h"
#include "snake.h"

static Snake *snake;

Snake *Snake_Init()
{
    snake = (Snake *)calloc(1, sizeof(Snake));
    snake->xDirection = 1;
    snake->yDirection = 0;

    snake->tail = NULL;
    Snake_AddBodyElement(SNAKE_START_X, SNAKE_START_Y);

    for (int i = 1; i < SNAKE_START_LENGTH; i++)
    {
        Snake_AddBodyElement(SNAKE_START_X - i, SNAKE_START_Y);
    }

    return snake;
}

Snake *Snake_Get()
{
    return snake;
}

void Snake_AddBodyElement(int x, int y)
{
    SnakeBody *body = (SnakeBody *)calloc(1, sizeof(SnakeBody));

    body->x = x;
    body->y = y;
    body->next = snake->tail;

    snake->tail = body;
}

void Snake_MoveLeft()
{
    if (snake->xDirection == 0)
    {
        snake->xDirection = -1;
        snake->yDirection = 0;
    }
}

void Snake_MoveRight()
{
    if (snake->xDirection == 0)
    {
        snake->xDirection = 1;
        snake->yDirection = 0;
    }
}

void Snake_MoveUp()
{
    if (snake->yDirection == 0)
    {
        snake->xDirection = 0;
        snake->yDirection = -1;
    }
}

void Snake_MoveDown()
{
    if (snake->yDirection == 0)
    {
        snake->xDirection = 0;
        snake->yDirection = 1;
    }
}

void Snake_Move(Game *game)
{
    SnakeBody *body = snake->tail;
    int x = body->x;
    int y = body->y;

    while (body->next != NULL)
    {
        body->x = body->next->x;
        body->y = body->next->y;

        body = body->next;
    }

    // body is now equal to the head
    body->x += snake->xDirection;
    body->y += snake->yDirection;

    // check if snake eats the fruit
    if (body->x == game->fruit->x && body->y == game->fruit->y)
    {
        Game_EatFruit();
        Snake_AddBodyElement(x, y);
    }

    // check collision with game borders or snake body
    game->isGameOver = Snake_Collision(body);
}

bool Snake_Collision(SnakeBody *head)
{
    SnakeBody *body = snake->tail;

    while (body->next != NULL)
    {
        // if collision with snake body
        if (body->x == head->x && body->y == head->y)
        {
            return true;
        }
        body = body->next;
    }

    // if touch the border
    if (head->x == 0 || head->x >= GAME_WIDTH ||
        head->y == 0 || head->y >= GAME_HEIGHT)
    {
        return true;
    }

    return false;
}