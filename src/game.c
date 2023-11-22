#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL_ttf.h"
#include <stdlib.h>
#include "log.h"
#include "game.h"
#include "renderer.h"
#include "snake.h"

static Game *game = NULL;

Game *Game_Init()
{
    game = (Game *)malloc(sizeof(Game));
    game->isRunning = false;
    game->isGameOver = false;
    game->speed = GAME_SPEED;
    game->score = 0;
    game->fruitTexture = NULL;
    game->fruitWidth = 0;
    game->fruitHeigh = 0;

    game->fruit = (Fruit *)malloc(sizeof(Fruit));
    game->fruit->x = 0;
    game->fruit->y = 0;

    game->snake = Snake_Init();

    return game;
}

Game *Game_Get()
{
    return game;
}

void Game_Start()
{
    Game_SpawnFruit();

    game->isRunning = true;
    game->isGameOver = false;

    // Animation loop
    while (game->isRunning && !game->isGameOver)
    {
        Game_Update();
        Game_Render();
        SDL_Delay(1000 / game->speed);
    }

    game->isRunning = false;
}

void Game_Stop()
{
    game->isRunning = false;
}

void Game_Update()
{
    if (game == NULL)
    {
        fprintf(stderr, "Game_Update failed: game is not initialized");
        return;
    }

    SDL_Event event;

    // Events management
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {

        case SDL_QUIT:
            // handling of close button
            game->isRunning = 0;
            break;

        case SDL_KEYDOWN:
            // keyboard API for key pressed
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                game->isRunning = 0;
                break;
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                Snake_MoveUp();
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                Snake_MoveLeft();
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                Snake_MoveDown();
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                Snake_MoveRight();
                break;
            default:
                break;
            }
        }
    }

    if (!game->isGameOver)
    {
        Snake_Move(game);
    }
}

void Game_Render()
{
    SDL_Renderer *renderer = Renderer_GetRenderer();
    SDL_Rect fruitRect;
    SDL_Surface *messageSurface;
    SDL_Texture *message;
    SDL_Rect Message_rect;
    SDL_Color white = {255, 255, 255, 255};
    TTF_Font *font;
    char scoreMessage[32];
    int texW, texH;

    if (game == NULL)
    {
        LogError("Game_processInput failed: game is not initialized");
        return;
    }

    if (TTF_Init() < 0)
    {
        LogError("Error from TTF_Init: %s", TTF_GetError());
        return;
    }

    if (!(font = TTF_OpenFont("assets/fonts/PixelifySans-Regular.ttf", 16)))
    {
        LogError("Error from TTF_OpenFont: %s", TTF_GetError());
        return;
    }

    SDL_RenderClear(renderer);

    // Display score
    sprintf(scoreMessage, "Score: %d", game->score);
    if (!(messageSurface = TTF_RenderText_Blended(font, scoreMessage, white)))
    {
        LogError("Error from TTF_RenderText_Blended: %s", TTF_GetError());
    }

    if (!(message = SDL_CreateTextureFromSurface(renderer, messageSurface)))
    {
        LogError("Error from SDL_CreateTextureFromSurface: %s", SDL_GetError());
        LogError(SDL_GetError());
    }

    SDL_QueryTexture(message, NULL, NULL, &texW, &texH);

    Message_rect.x = 5;
    Message_rect.y = 5;
    Message_rect.w = texW;
    Message_rect.h = texH;

    SDL_RenderCopy(renderer, message, NULL, &Message_rect);

    // Load the fruit texture if not loaded yet and draw it
    if (game->fruitTexture == NULL)
    {
        game->fruitTexture = IMG_LoadTexture(renderer, APPLE_IMG_PATH);
        SDL_QueryTexture(game->fruitTexture, NULL, NULL, &game->fruitWidth, &game->fruitHeigh);
    }

    fruitRect.x = game->fruit->x * CELL_WIDTH;
    fruitRect.y = game->fruit->y * CELL_HEIGHT;
    fruitRect.w = CELL_WIDTH;
    fruitRect.h = CELL_HEIGHT;

    SDL_RenderCopy(renderer, game->fruitTexture, NULL, &fruitRect);

    // Render Snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
    for (SnakeBody *snake = game->snake->tail; snake; snake = snake->next)
    {
        SDL_Rect rect = {
            snake->x * CELL_WIDTH,
            snake->y * CELL_HEIGHT,
            CELL_WIDTH,
            CELL_HEIGHT};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Set background color
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 0);

    SDL_RenderPresent(renderer);
}

void Game_SpawnFruit()
{
    if (game == NULL)
    {
        LogError("Game_SpawnFruit failed: game is not initialized");
        return;
    }

    if (game->fruit == NULL)
    {
        LogError("Game_SpawnFruit failed: fruit is not initialized");
        return;
    }

    // Avoid to spawn the fruit too close to the border
    game->fruit->x = BORDER_OFFSET + (rand() % (GAME_WIDTH - (BORDER_OFFSET * 2) + 1));
    game->fruit->y = BORDER_OFFSET + (rand() % (GAME_HEIGHT - (BORDER_OFFSET * 2) + 1));
}

void Game_EatFruit()
{
    game->score++;
    game->speed++;

    Game_SpawnFruit();
}

void Game_Destroy()
{
    if (!game)
        return;

    // Destroy the fruit texture and free fruit memory
    SDL_DestroyTexture(game->fruitTexture);
    free(game->fruit);

    // free game memory
    free(game);
}