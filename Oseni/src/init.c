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

	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) < 0);
	{
		printf("Window and renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /*Set draw colour to black*/
	SDL_RenderClear(renderer); /*Clear the screen with black*/

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /*Set draw color to white*/
	SDL_RenderDrawPoint(
			renderer,
		       640 / 2,
		       480 / 2); /*Draw a white point at the center of the screen*/

	SDL_RenderPresent(renderer); /*update the screen*/

	SDL_Delay(10000); /*Waits for 10 seconds*/

	SDL_DestroyRenderer(renderer); /*Cleans up window*/
	SDL_DestroyWindow(window);
	SDL_Quit();


	return (0);
}
