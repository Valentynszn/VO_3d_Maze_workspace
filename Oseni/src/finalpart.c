#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAP_WIDTH 20
#define MAP_HEIGHT 13
#define TILE_SIZE 40
#define FOV_ANGLE 60 * M_PI / 180.0f  /*Field of view angle in radians*/
#define NUM_RAYS 100
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

typedef struct
{
	float x;
	float y;
} Vector2;

typedef struct
{
	Vector2 position;
	float angle;
} Player;


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

void castRays(SDL_Renderer *renderer, Player *player)
{
	float ray_angle_increment = FOV_ANGLE / (float)NUM_RAYS;
	float ray_angle = player->angle - FOV_ANGLE / 2.0f;

	for (int ray = 0; ray < NUM_RAYS; ray++)
	{
		/*Calculate ray endpoints*/
		float ray_dir_x = cos(ray_angle);
		float ray_dir_y = sin(ray_angle);

		/*Initialize ray variables*/
		float x = player->position.x;
		float y = player->position.y;
		bool hit = false;
		float distance = 0;

		/*Ray casting loop*/
		while (!hit && distance < 1000)
		{
			x += ray_dir_x;
			y += ray_dir_y;
			distance += 1;

			int map_x = (int)(x / TILE_SIZE);
			int map_y = (int)(y / TILE_SIZE);

			if (map[map_y][map_x] != 0)
			{
				hit = true;
				/* Calculate perpendicular distance to avoid fish-eye effect */
				float perpendicular_distance = distance * cos(ray_angle - player->angle);
				/* Calculate the height of the wall slice */
				int wall_height = (int)(SCREEN_HEIGHT / perpendicular_distance);

				/* Calculate the start and end y-coordinates for the wall slice */
				int wall_start = (SCREEN_HEIGHT / 2) - (wall_height / 2);
				int wall_end = (SCREEN_HEIGHT / 2) + (wall_height / 2);


				/* Set color based on the wall type */
				if (map[map_y][map_x] == 1)
				{
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); /* Green */
				}
				else if (map[map_y][map_x] == 6)
				{
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /* Red */
				}
				else if (map[map_y][map_x] == 7)
				{
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); /* Blue */
				}

				/* Draw the wall slice */
				SDL_RenderDrawLine(renderer, ray * (SCREEN_WIDTH / NUM_RAYS), wall_start, ray * (SCREEN_WIDTH / NUM_RAYS), wall_end);

			}

		}
		ray_angle += ray_angle_increment;
	}
}

int main(void)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int bot_size  = 20; /*size of the guy at the center*/
	Player player = {{1280 / 2 - bot_size / 2, 600 / 2 - bot_size / 2},0.0f}; /* Start at the center of the screen */
	SDL_Rect rect = {player.position.x, player.position.y, bot_size, bot_size};


	float angle = 0.0f; /* Initial angle for rotation */
	float speed = 2.5f; /* Movement speed */
	bool showMap = false; /* Flag to toggle map display */

	/* Array to keep track of key states */
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

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

	/* Load the map */
	renderMap(renderer, map);

	/*Event loop*/
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
		/* Initialize new_rect to the current position before moving */
		SDL_Rect new_rect_x = rect;
		SDL_Rect new_rect_y = rect;

		if (keystate[SDL_SCANCODE_W])
		{
			new_rect_x.x += speed * cos(player.angle); /* Move forward */
			new_rect_y.y += speed * sin(player.angle);
		}
		if (keystate[SDL_SCANCODE_A])
		{
			player.angle -= 15.0f * M_PI / 180.0f; /* Rotate left by 30 degrees */
		}
		if (keystate[SDL_SCANCODE_D])
		{
			player.angle += 15.0f * M_PI / 180.0f; /* Rotate right by 30 degrees*/
		}
		if (keystate[SDL_SCANCODE_S])
		{
			new_rect_x.x -= speed * cos(player.angle);
			new_rect_y.y -= speed * sin(player.angle);

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


		/* Wrap angle to stay within (0, 2*pi)*/
		if (player.angle < 0.0f)
			player.angle += 2 * M_PI;
		else if (player.angle >= 2 * M_PI)
			player.angle -= 2 * M_PI;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /*Set draw colour to black*/
		SDL_RenderClear(renderer); /*Clear the screen with black*/
		if (showMap)
		{
			renderMap(renderer, map); /* Render the map */
		}

		/* Calculate center of the bot */
		SDL_Point center = {rect.x + bot_size / 2, rect.y + bot_size / 2};

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /*Set bot color to white*/
		SDL_Rect bot_rect = {center.x - bot_size / 2, center.y - bot_size / 2, bot_size, bot_size};
		SDL_RenderFillRect(renderer, &bot_rect);

		/* Draw the direction line */
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /* Set line color to white*/
		int line_length = 30; /* Length of the direction line */
		SDL_Point line_end = {center.x + line_length * cos(angle), center.y + line_length * sin(angle)};
		SDL_RenderDrawLine(renderer, center.x, center.y, line_end.x, line_end.y);

		player.position.x = rect.x; /* Update player's position */
		player.position.y = rect.y;

		castRays(renderer, &player); /*Draw the rays*/

		SDL_RenderPresent(renderer); /*update the screen*/

		SDL_Delay(10); /*adjust delay for frame rate*/
}


	 /*Cleans up window*/
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return (0);
}
