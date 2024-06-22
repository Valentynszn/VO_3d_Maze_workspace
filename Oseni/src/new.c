#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define MAP_WIDTH 20
#define MAP_HEIGHT 13
#define TILE_SIZE 40

char map[MAP_HEIGHT][MAP_WIDTH] = {
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6},
    {6, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6},
    {6, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 6, 0, 7, 7, 0, 0, 0, 0, 6},
    {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 7, 0, 6},
    {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 6},
    {6, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 1, 0, 0, 0, 0, 7, 7, 0, 6},
    {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 1, 0, 6},
    {6, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 1, 0, 6},
    {6, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
};

void renderMap(SDL_Renderer *renderer, char map[MAP_HEIGHT][MAP_WIDTH])
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            SDL_Rect tile = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            if (map[i][j] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); /* Green colored walls */
                SDL_RenderFillRect(renderer, &tile);
            }
            else if (map[i][j] == 6)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /* Red colored walls */
                SDL_RenderFillRect(renderer, &tile);
            }
            else if (map[i][j] == 7)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); /* Blue colored walls */
                SDL_RenderFillRect(renderer, &tile);
            }
        }
    }
}

bool checkCollision(SDL_Rect *rect, char map[MAP_HEIGHT][MAP_WIDTH])
{
    int left = rect->x / TILE_SIZE;
    int right = (rect->x + rect->w) / TILE_SIZE;
    int top = rect->y / TILE_SIZE;
    int bottom = (rect->y + rect->h) / TILE_SIZE;

    if (left < 0 || right >= MAP_WIDTH || top < 0 || bottom >= MAP_HEIGHT)
        return true;

    if (map[top][left] != 0 || map[top][right] != 0 || map[bottom][left] != 0 || map[bottom][right] != 0)
        return true;

    return false;
}

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int bot_size = 20; /* Size of the player */
    SDL_Rect rect = {1280 / 2 - bot_size / 2, 600 / 2 - bot_size / 2, bot_size, bot_size}; /* Start at the center of the screen */

    /* Direction vectors */
    double dirX = -1.0, dirY = 0.0;      // Initial direction vector
    double planeX = 0.0, planeY = 0.66;  // Camera plane (perpendicular to direction vector)

    /* Movement and rotation variables */
    double moveSpeed = 0.05;   // Adjust the movement speed
    double rotSpeed = 0.03;    // Adjust the rotation speed

    /* Array to keep track of key states */
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Raycasting",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1280, 600,
        SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    /* Load the map */
    renderMap(renderer, map);

    /* Event loop */
    SDL_Event event;
    int quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        /* Handle movement and rotation */
        double oldDirX = dirX;
        double oldPlaneX = planeX;

        if (keystate[SDL_SCANCODE_UP])
        {
            if (!checkCollision(&rect, map))
            {
                rect.x += dirX * moveSpeed;
                rect.y += dirY * moveSpeed;
            }
        }
        if (keystate[SDL_SCANCODE_DOWN])
        {
            if (!checkCollision(&rect, map))
            {
                rect.x -= dirX * moveSpeed;
                rect.y -= dirY * moveSpeed;
            }
        }
        if (keystate[SDL_SCANCODE_LEFT])
        {
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }
        if (keystate[SDL_SCANCODE_RIGHT])
        {
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        }

        /* Clear screen */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        /* Draw the player */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);

        /* Draw the map */
        renderMap(renderer, map);

        /* Update screen */
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

