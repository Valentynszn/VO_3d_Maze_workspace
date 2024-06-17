#include <SDL2/SDL.h> // SDL2 main header

int main(int argc, char *argv[])
{
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Main loop
    int quit = 0;
    SDL_Event event;
    while (!quit)
    {
        // Event loop
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        // Render here

        // Update window
        SDL_UpdateWindowSurface(window);
    }

    // Cleanup and quit SDL2
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
