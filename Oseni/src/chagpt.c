#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAP_WIDTH 20
#define MAP_HEIGHT 13
#define TILE_SIZE 40
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 600

/**
 * Main function to initialize SDL, create a window and renderer, draw a point in the center of the screen..
 *
 * Return: 0
 */

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

void castRays(SDL_Renderer *renderer, float playerX, float playerY, float playerAngle)
{
    /* Define the player's direction vector */
    float dirX = cos(playerAngle);
    float dirY = sin(playerAngle);

    /* Define the camera plane (this determines the field of view) */
    float planeX = -dirY * 0.66;
    float planeY = dirX * 0.66;

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        /* Calculate the ray position and direction */
        float cameraX = 2 * x / (float)SCREEN_WIDTH - 1; /* x-coordinate in camera space */
        float rayDirX = dirX + planeX * cameraX;
        float rayDirY = dirY + planeY * cameraX;

        /* Which box of the map we're in */
        int mapX = (int)playerX / TILE_SIZE;
        int mapY = (int)playerY / TILE_SIZE;

        /* Length of the ray from one x or y-side to the next x or y-side */
        float deltaDistX = fabs(1 / rayDirX);
        float deltaDistY = fabs(1 / rayDirY);

        /* Calculate step and initial sideDist */
        int stepX, stepY;
        float sideDistX, sideDistY;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (playerX - mapX * TILE_SIZE) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = ((mapX + 1) * TILE_SIZE - playerX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (playerY - mapY * TILE_SIZE) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = ((mapY + 1) * TILE_SIZE - playerY) * deltaDistY;
        }

        /* Perform DDA */
        bool hit = false;
        int side; /* Was a NS or a EW wall hit? */
        while (!hit)
        {
            /* Jump to next map square, either in x-direction, or in y-direction */
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            /* Check if ray has hit a wall */
            if (map[mapY][mapX] > 0)
                hit = true;
        }

        /* Calculate distance to the point of impact */
        float perpWallDist;
        if (side == 0)
            perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;

        /* Calculate height of line to draw on screen */
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        /* Calculate lowest and highest pixel to fill in current stripe */
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT)
            drawEnd = SCREEN_HEIGHT - 1;

        /* Choose wall color */
        SDL_Color color;
        if (map[mapY][mapX] == 1)
            color = (SDL_Color){0, 255, 0, 255}; /* Green */
        else if (map[mapY][mapX] == 6)
            color = (SDL_Color){255, 0, 0, 255}; /* Red */
        else if (map[mapY][mapX] == 7)
            color = (SDL_Color){0, 0, 255, 255}; /* Blue */

        /* Set the color and draw the vertical stripe */
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int bot_size = 20; /* Size of the player */
    SDL_Rect rect = {SCREEN_WIDTH / 2 - bot_size / 2, SCREEN_HEIGHT / 2 - bot_size / 2, bot_size, bot_size}; /* Start at the center of the screen */

    /* Declared new_rect outside the event loop */
    SDL_Rect new_rect_x, new_rect_y;

    float angle = 0.0f; /* Initial angle for rotation */
    float speed = 2.5f; /* Movement speed */
    bool showMap = false; /* Flag to toggle map display */

    /* Player position and angle */
    float playerX = rect.x;
    float playerY = rect.y;
    float playerAngle = angle;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return (1);
    }

    window = SDL_CreateWindow(
        "tutorial",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return (1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return (1);
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
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_q)
                {
                    showMap = !showMap; /* Toggle map display */
                }
            }
        }

        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        /* Initialize new_rect to the current position before moving */
        new_rect_x = rect;
        new_rect_y = rect;

        if (keystate[SDL_SCANCODE_W])
        {
            new_rect_x.x += speed * cos(angle); /* Move forward */
            new_rect_y.y += speed * sin(angle);
        }
        if (keystate[SDL_SCANCODE_A])
        {
            angle -= 15.0f * M_PI / 180.0f; /* Rotate left by 15 degrees */
        }
        if (keystate[SDL_SCANCODE_D])
        {
            angle += 15.0f * M_PI / 180.0f; /* Rotate right by 15 degrees */
        }
        if (keystate[SDL_SCANCODE_S])
        {
            new_rect_x.x -= speed * cos(angle);
            new_rect_y.y -= speed * sin(angle);
        }

        /* Check for collisions */
        if (!checkCollision(&new_rect_x, map))
        {
            rect.x = new_rect_x.x; /* Update x position only if no collision */
        }
        if (!checkCollision(&new_rect_y, map))
        {
            rect.y = new_rect_y.y; /* Update y position only if no collision */
        }

        /* Update player position */
        playerX = rect.x + bot_size / 2;
        playerY = rect.y + bot_size / 2;
        playerAngle = angle;

        /* Wrap angle to stay within (0, 2*pi) */
        if (angle < 0.0f)
            angle += 2 * M_PI;
        else if (angle >= 2 * M_PI)
            angle -= 2 * M_PI;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /* Set draw color to black */
        SDL_RenderClear(renderer); /* Clear the screen with black */

        if (showMap)
        {
            renderMap(renderer, map); /* Render the map */
        }

        /* Calculate center of the bot */
        SDL_Point center = {rect.x + bot_size / 2, rect.y + bot_size / 2};

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /* Set bot color to white */
        SDL_Rect botRect = {center.x - bot_size / 2, center.y - bot_size / 2, bot_size, bot_size};
        SDL_RenderFillRect(renderer, &botRect);

        /* Draw the direction line */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /* Set line color to white */
        int line_length = 30; /* Length of the direction line */
        SDL_Point line_end = {center.x + line_length * cos(angle), center.y + line_length * sin(angle)};
        SDL_RenderDrawLine(renderer, center.x, center.y, line_end.x, line_end.y);

        castRays(renderer, playerX, playerY, playerAngle);

        SDL_RenderPresent(renderer); /* Update the screen */

        SDL_Delay(10); /* Adjust delay for frame rate */
    }

    /* Clean up window */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return (0);
}

