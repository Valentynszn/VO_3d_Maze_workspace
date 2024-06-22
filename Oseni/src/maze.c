#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#define MAP_WIDTH 20
#define MAP_HEIGHT 13
#define TILE_SIZE 40
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 600

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

void renderMap(SDL_Renderer *renderer, char map[MAP_HEIGHT][MAP_WIDTH]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            SDL_Rect tile = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            if (map[i][j] == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); /* Green colored walls */
                SDL_RenderFillRect(renderer, &tile);
            } else if (map[i][j] == 6) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /* Red colored walls */
                SDL_RenderFillRect(renderer, &tile);
            } else if (map[i][j] == 7) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); /* Blue colored walls */
                SDL_RenderFillRect(renderer, &tile);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Rect playerRect = {SCREEN_WIDTH / 2 - 10 / 2, SCREEN_HEIGHT / 2 - 10 / 2, 10, 10}; /* Start at the center of the screen */

    float playerX = playerRect.x;
    float playerY = playerRect.y;
    float playerDirX = -1.0f;
    float playerDirY = 0.0f;
    float planeX = 0.0f;
    float planeY = 0.66f;

    float moveSpeed = 0.1f;
    float rotSpeed = 0.05f;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Raycasting Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        /* Move forward if no wall in front */
        if (keystate[SDL_SCANCODE_W]) {
            if (map[(int)(playerX + playerDirX * moveSpeed)][(int)playerY] == 0) {
                playerX += playerDirX * moveSpeed;
            }
            if (map[(int)playerX][(int)(playerY + playerDirY * moveSpeed)] == 0) {
                playerY += playerDirY * moveSpeed;
            }
        }

        /* Move backward if no wall behind */
        if (keystate[SDL_SCANCODE_S]) {
            if (map[(int)(playerX - playerDirX * moveSpeed)][(int)playerY] == 0) {
                playerX -= playerDirX * moveSpeed;
            }
            if (map[(int)playerX][(int)(playerY - playerDirY * moveSpeed)] == 0) {
                playerY -= playerDirY * moveSpeed;
            }
        }

        /* Rotate right */
        if (keystate[SDL_SCANCODE_D]) {
            float oldDirX = playerDirX;
            playerDirX = playerDirX * cos(-rotSpeed) - playerDirY * sin(-rotSpeed);
            playerDirY = oldDirX * sin(-rotSpeed) + playerDirY * cos(-rotSpeed);
            float oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        }

        /* Rotate left */
        if (keystate[SDL_SCANCODE_A]) {
            float oldDirX = playerDirX;
            playerDirX = playerDirX * cos(rotSpeed) - playerDirY * sin(rotSpeed);
            playerDirY = oldDirX * sin(rotSpeed) + playerDirY * cos(rotSpeed);
            float oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }

        /* Clear screen */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        /* Render map */
        renderMap(renderer, map);

        /* Draw player */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        /* Update screen */
        SDL_RenderPresent(renderer);
    }

    /* Cleanup */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

