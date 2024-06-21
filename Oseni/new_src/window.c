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
	SDL_Rect bot = {1280 / 2 - bot_size / 2, 600 / 2 - bot_size / 2, bot_size, bot_size}; /* Start at the center of the screen */
	float angle = 0.0f; /* Initial angle for rotation */
	float speed = 2.5f; /* Movement speed */

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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
						case SDLK_x:
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


		/* Wrap angle to stay within (0, 2*pi)*/
		if (angle < 0.0f)
			angle += 2 * M_PI;
		else if (angle >= 2 * M_PI)
			angle -= 2 * M_PI;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /*Set draw colour to black*/
	SDL_RenderClear(renderer); /*Clear the screen with black*/

	int centerX = 1280 / 2;
	int centerY = 720 / 2;
	SDL_Rect bot = {centerX - bot_size / 2,
			centerY - bot_size / 2,
			bot_size, bot_size};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /*Set draw color to red*/
	SDL_RenderFillRect(renderer, &bot);

	/*Calculate the center of the bot*/
	int botCenterX = bot.x + bot.w / 2;
	int botCenterY = bot.y + bot.h / 2;

	/*Set the draw color to white and draw the line*/
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // White color
	SDL_RenderDrawLine(renderer, centerX, centerY, botCenterX, botCenterY);


	SDL_RenderPresent(renderer); /*update the screen*/
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return (0);
}
