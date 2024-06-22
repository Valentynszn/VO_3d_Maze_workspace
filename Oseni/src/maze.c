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
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); /*Green colored walls*/
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
    SDL_Rect playerRect = {1280 / 2 - 10 / 2, 600 / 2 - 10 / 2, 10, 10}; /* Start at the center of the screen */

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

        /* Draw rays for each x-coordinate of the screen */
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            /* Calculate ray position and direction */
            float cameraX = 2 * x / (float)SCREEN_WIDTH - 1; /* X-coordinate in camera space */
            float rayDirX = playerDirX + planeX * cameraX;
            float rayDirY = playerDirY + planeY * cameraX;

            /* Which box of the map we're in */
            int mapX = (int)playerX;
            int mapY = (int)playerY;

            /* Length of ray from current position to next x or y-side */
            float sideDistX;
            float sideDistY;

            /* Length of ray from one x or y-side to next x or y-side */
            float deltaDistX = fabs(1 / rayDirX);
            float deltaDistY = fabs(1 / rayDirY);
            float perpWallDist;

            /* What direction to step in x or y-direction (either +1 or -1) */
            int stepX;
            int stepY;

            int hit = 0; /* Was there a wall hit? */
            int side;    /* Was a NS or a EW wall hit? */

            /* Calculate step and initial sideDist */
            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (playerX - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - playerX) * deltaDistX;
            }
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (playerY - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - playerY) * deltaDistY;
            }

            /* Perform DDA */
            while (hit == 0) {
                /* Jump to next map square, OR in x-direction, OR in y-direction */
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0; /* NS wall */
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1; /* EW wall */
                }

                /* Check if ray has hit a wall */
                if (map[mapX][mapY] > 0) {
                    hit = 1;
                }
            }

            /* Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!) */
            if (side == 0) {
                perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
            } else {
                perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;
            }

            /* Calculate height of line to draw on screen */
            int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

            /* Calculate lowest and highest pixel to fill in current stripe */
            int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawStart < 0) {
                drawStart = 0;
            }
            int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawEnd >= SCREEN_HEIGHT) {
                drawEnd = SCREEN_HEIGHT - 1;
            }

            /* Choose wall color based on the type */
            SDL_Color color;
            switch (map[mapX][mapY]) {
                case 1: /* Green wall */
                    color = (SDL_Color){0, 255, 0, 255};
                    break;
                case 6: /* Red wall */
                    color = (SDL_Color){255, 0, 0, 255};
                    break;
                case 7: /* Blue wall */
                    color = (SDL_Color){0, 0, 255, 255};
                    break;
                default: /* Default wall color */
                    color = (SDL_Color){255, 255, 255, 255};
                    break;
            }

            /* Draw the vertical line for this x-coordinate */
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
        }

        /* Render the map */
        renderMap(renderer, map);

        /* Update the screen */
        SDL_RenderPresent(renderer);

        /* Delay for frame rate control */
        SDL_Delay(10);
    }

    /* Cleanup and exit */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

