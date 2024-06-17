#include <SDL2/SDL.h>
#include <stdio.h>

/**
 * Main function to initialize SDL, create a window and renderer, draw a point in the center of the screen, and wait for 10 seconds.
 *
 * Return: 0
 */

int main(void)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int bot_size  = 40; /*size of the guy at the center*/

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return (1);
	}

	window = SDL_CreateWindow(
			"tutorial",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			1280, 720,
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

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /*Set draw colour to black*/
	SDL_RenderClear(renderer); /*Clear the screen with black*/

	int centerX = 1280 / 2;
	int centerY = 7200 / 2;
	SDL_Rect rect = {centerX - bot_size / 2, centerY - bot_size / 2, bot_size, bot_size};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /*Set draw color to red*/
	SDL_RenderFillRect(renderer, &rect);

	SDL_RenderPresent(renderer); /*update the screen*/

	SDL_Delay(10000); /*Waits for 10 seconds*/

	 /*Cleans up window*/
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return (0);
}
