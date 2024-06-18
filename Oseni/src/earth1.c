#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAP_WIDTH 16
#define MAP_HEIGHT 6
#define TILE_SIZE 40

/**
 * Main function to initialize SDL, create a window and renderer, draw a point in the center of the screen..
 *
 * Return: 0
 */

void loadMap(char map[MAP_HEIGHT][MAP_WIDTH])
{
	FILE *file = fopen("world.txt", "r");
	if (file == NULL)
	{
		printf("Could not open map file!\n");
		exit(1);
	}

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			map[i][j] = fgetc(file);
		}
		fgetc(file); /*Skips the newline character*/
	}
	fclose(file);
}

void renderMap(SDL_Renderer *renderer, char map[MAP_HEIGHT][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (map[i][j] == '1')
			{
				SDL_Rect tile = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); /*Green colored walls*/
				SDL_RenderFillRect(renderer, &tile);
			}
		}
	}
}

int main(void)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int bot_size  = 20; /*size of the guy at the center*/
	SDL_Rect rect = {1280 / 2 - bot_size / 2, 600 / 2 - bot_size / 2, bot_size, bot_size}; /* Start at the center of the screen */
	float angle = 0.0f; /* Initial angle for rotation */
	float speed = 5.0f; /* Movement speed */

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return (1);
	}

	window = SDL_CreateWindow(
			"tutorial",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			1280, 600,
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

	char map[MAP_HEIGHT][MAP_WIDTH];
	loadMap(map);

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
							rect.x += speed * cos(angle); /* Move forward */
							rect.y += speed * sin(angle);
							break;
						case SDLK_a:
							angle -= 30.0f * M_PI / 180.0f; /* Rotate left by 30 degrees */
							break;
						case SDLK_d:
							angle += 30.0f * M_PI / 180.0f; /* Rotate right by 30 degrees */
							break;
						case SDLK_x:
							rect.x -= speed * cos(angle);
							rect.y -= speed * sin(angle);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}


		/* Wrap angle to stay within (0, 2*pi)*/
		if (angle < 0.0f)
			angle += 2 * M_PI;
		else if (angle >= 2 * M_PI)
			angle -= 2 * M_PI;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /*Set draw colour to black*/
		SDL_RenderClear(renderer); /*Clear the screen with black*/

		renderMap(renderer, map); /* Render the map */

		/* Calculate center of the bot */
		SDL_Point center = {rect.x + bot_size / 2, rect.y + bot_size / 2};

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /*Set draw color to red*/
		SDL_Rect rect = {center.x - bot_size / 2, center.y - bot_size / 2, bot_size, bot_size};
		SDL_RenderFillRect(renderer, &rect);

		/* Draw the direction line */
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /* Set draw color to red*/
		int line_length = 30; /* Length of the direction line */
		SDL_Point line_end = {center.x + line_length * cos(angle), center.y + line_length * sin(angle)};
		SDL_RenderDrawLine(renderer, center.x, center.y, line_end.x, line_end.y);

		SDL_RenderPresent(renderer); /*update the screen*/

		SDL_Delay(10); /*adjust delay for frame rate*/
	}


	 /*Cleans up window*/
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return (0);
}