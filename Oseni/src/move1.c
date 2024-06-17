#include <SDL2/SDL.h>
#include <stdio.h>

/**
 * Main function to initialize SDL, create a window and renderer, draw a point in the center of the screen..
 *
 * Return: 0
 */

int main(void)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int bot_size  = 20; /*size of the guy at the center*/
	SDL_Rect rect = {0, 0, bot_size, bot_size};

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
				case SDLK_w:
					rect.y -= 10;	/*Move up*/
					break;
				case SDLK_a:
					rect.x -= 10;
					break;
				case SDLK_d:
					rect.x += 10;
					break;
				case SDLK_x:
					rect.y += 10;
					break;
				default:
					break;
			}
			break;
		}
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /*Set draw colour to black*/
	SDL_RenderClear(renderer); /*Clear the screen with black*/

	int centerX = 1280 / 2;
	int centerY = 720 / 2;
	SDL_Rect box = {centerX - bot_size / 2, centerY - bot_size / 2, bot_size, bot_size};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /*Set draw color to red*/
	SDL_RenderFillRect(renderer, &rect);


	SDL_RenderPresent(renderer); /*update the screen*/

	SDL_Delay(10); /*adjust delay for frame rate*/

	 /*Cleans up window*/
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return (0);
}
