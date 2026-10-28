#include <SDL3/SDL.h>
#include <iostream>

int main() {
    // This if statement checks for if SDL initialized
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL initialization failed: "
                  << SDL_GetError() << '\n';
        return 1;
    }

    // This thing creates a pointer to the window that is created with pixel sizes 800 by 600
    SDL_Window* window = SDL_CreateWindow(
        "Ceometry Dash++",
        800,
        600,
        0
    );

    // Check if window creation failed or not
    if (!window) {
        std::cerr << "Window creation failed: "
                  << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    // Run the program and get events that happen
    bool running = true;
    SDL_Event event;

    while (running) {
        // This while loop continues when there are events that happened
        while (SDL_PollEvent(&event)) {
            // Check if user wants to quit
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
    }

    // Close the window and stuff like that
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
