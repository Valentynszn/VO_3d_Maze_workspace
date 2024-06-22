#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAP_WIDTH 20
#define MAP_HEIGHT 13
#define TILE_SIZE 40
#define SCREEN_HEIGHT 1280
#define SCREEN_WIDTH 600

void renderMap(SDL_Renderer *renderer, char map[MAP_HEIGHT][MAP_WIDTH]);
bool checkCollision(SDL_Rect *bot, char map[MAP_HEIGHT][MAP_WIDTH]);
void renderGrid(SDL_Renderer *renderer, int windowWidth, int windowHeight, int tileSize);

/**
 * Main function to initialize SDL, create a window and renderer, draw a point in the center of the screen..
 *
 * Return: 0
 */

int main(void)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int bot_size  = 40; /*size of the guy at the center*/
	SDL_Rect bot = {SCREEN_WIDTH / 2 - bot_size / 2, SCREEN_HEIGHT / 2 - bot_size / 2, bot_size, bot_size}; /* Start at the center of the screen */
	float angle = 0.0f; /* Initial angle for rotation */
	float speed = 2.5f; /* Movement speed */
	SDL_Rect new_bot_x, new_bot_y;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return (1);
	}

	window = SDL_CreateWindow(
			"tutorial",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			1280, 600,
			SDL_WINDOW_SHOWN
			);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

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

void renderGrid(SDL_Renderer *renderer, int windowWidth, int windowHeight, int tileSize)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int x = 0; x < windowWidth; x += tileSize)
	{
		SDL_RenderDrawLine(renderer, x, 0, x, windowHeight);
	}

	for (int y = 0; y < windowHeight; y += tileSize)
	{
		SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
	}
}

void renderMap(SDL_Renderer *renderer, char map[MAP_HEIGHT][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			SDL_Rect tile = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
			if (map[i][j] == 1)
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); /*Green colored walls*/
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

bool checkCollision(SDL_Rect *bot, char map[MAP_HEIGHT][MAP_WIDTH])
{
	int left = bot->x / TILE_SIZE;
	int right = (bot->x + bot->w) / TILE_SIZE;
	int top = bot->y / TILE_SIZE;
	int bottom = (bot->y + bot->h) / TILE_SIZE;

	if (left < 0 || right >= MAP_WIDTH || top < 0 || bottom >= MAP_HEIGHT)
		return true;

	if (map[top][left] != 0 || map[top][right] != 0 || map[bottom][left] != 0 || map[bottom][right] != 0)
		return true;

	return false;
}

	/*Event loop*/
	SDL_Event event;
	int quit = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_w:
							bot.x += speed * cos(angle); /* Move forward */
							bot.y += speed * sin(angle);
							break;
						case SDLK_a:
							angle -= 30.0f * M_PI / 180.0f; /* Rotate left by 30 degrees */
							break;
						case SDLK_d:
							angle += 30.0f * M_PI / 180.0f; /* Rotate right by 30 degrees */
							break;
						case SDLK_s:
							bot.x -= speed * cos(angle);
							bot.y -= speed * sin(angle);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		/* Check for collisions */
		if (!checkCollision(&new_bot_x, map))
		{
			bot.x = new_bot_x.x; /* Update x position only if no collision */
		}
		if (!checkCollision(&new_bot_y, map))
		{
			bot.y = new_bot_y.y; /* Update y position only if no collision */
		}


		/* Wrap angle to stay within (0, 2*pi)*/
		if (angle < 0.0f)
			angle += 2 * M_PI;
		else if (angle >= 2 * M_PI)
			angle -= 2 * M_PI;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /*Set draw colour to black*/
	renderGrid(renderer, 1280, 600, TILE_SIZE);
	SDL_RenderClear(renderer); /*Clear the screen with black*/
	renderMap(renderer, map);

	int centerX = 1280 / 2;
	int centerY = 600 / 2;
	SDL_Rect bot = {SCREEN_WIDTH / 2 - bot_size / 2, SCREEN_HEIGHT / 2 - bot_size / 2, bot_size, bot_size}; /* Start at the center of the screen */
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /*Set draw color to white*/
	SDL_RenderFillRect(renderer, &bot);

	/*Calculate the center of the bot*/
	int botCenterX = bot.x + bot.w / 2;
	int botCenterY = bot.y + bot.h / 2;

	 /* Calculate the end point of the line at the edge of the bot */
        int lineEndX = botCenterX + (bot_size / 2) * cos(angle);
        int lineEndY = botCenterY + (bot_size / 2) * sin(angle);

	/*Set the draw color to white and draw the line*/
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /*red color*/
	SDL_RenderDrawLine(renderer, botCenterX, botCenterY, lineEndX, lineEndY);


	SDL_RenderPresent(renderer); /*update the screen*/
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return (0);
}
