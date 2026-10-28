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

    // Create renderer and see if creation worked or not
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer creation failed: "
                << SDL_GetError() << '\n';

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create two rectangles, one for the ground, one for the floor
    // The parameters are as follows {distance from left of screen, distance from top of screen, width, height}
    SDL_FRect floorRect{0.0f, 500.0f, 800.0f, 100.0f};
    SDL_FRect playerRect{150.0f, 460.0f, 40.0f, 40.0f};
    
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

        // Paint the background dark blue
        // Color values are red, green, blue, alpha, where 255 alpha is fully solid, no transparency
        SDL_SetRenderDrawColor(renderer, 20, 25, 40, 255);

        // Wipe screen clean
        SDL_RenderClear(renderer);

        // Draw the floor gray
        SDL_SetRenderDrawColor(renderer, 90, 95, 110, 255);
        SDL_RenderFillRect(renderer, &floorRect);

        // Draw the player yellow
        SDL_SetRenderDrawColor(renderer, 255, 210, 50, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        // Show the completed frame
        SDL_RenderPresent(renderer);

        // Temporarily limit how quickly the loop repeats
        // Pauses 16 ms, which equates to a bit more than 60 fps
        SDL_Delay(16);
    }

    // Close the window, renderer, stuff like that
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
