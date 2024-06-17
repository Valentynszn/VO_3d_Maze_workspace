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
	SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /*Set draw colour to black*/
	SDL_RenderClear(renderer); /*Clear the screen with black*/

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /*Set draw color to red*/
	SDL_RenderDrawPoint(
			renderer,
		       640 / 2,
		       480 / 2); /*Draw a red point at the center of the screen*/

	SDL_RenderPresent(renderer); /*update the screen*/

	SDL_Delay(10000); /*Waits for 10 seconds*/

	SDL_DestroyRenderer(renderer); /*Cleans up window*/
	SDL_DestroyWindow(window);
	SDL_Quit();


	return (0);
}
