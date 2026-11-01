#include <SDL3/SDL.h>
#include <iostream>
#include "Player.hpp"

int main() {
    // This if statement checks for if SDL initialized
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL initialization failed: "
                  << SDL_GetError() << '\n';
        return 1;
    }

    // This thing creates a pointer to the window that is created with pixel sizes 800 by 600
    SDL_Window* window = SDL_CreateWindow("Ceometry Dash++", 800, 600, 0);

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

    // Create objects
    Player player{playerRect.y, playerRect.h};


    
    // Run the program and get events that happen
    bool running = true;
    SDL_Event event;

    // Physics advance in steps of 1/120 of a second (Ticks per second)
    // This way movement stays consistent when rendering speed changes
    constexpr double TPS = 1.0 / 120.0;
    double accumulator = 0.0;
    Uint64 previousTime = SDL_GetTicksNS();
    constexpr float dt = static_cast<float>(TPS);

    // Define frame target
    constexpr Uint64 targetFPS = 60;
    constexpr Uint64 targetFrameNS = 1'000'000'000ULL / targetFPS;

    while (running) {
        Uint64 frameStart = SDL_GetTicksNS();

        // This while loop checks the events that happened since the last frame
        while (SDL_PollEvent(&event)) {
            // Check if user wants to quit
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            // Check for input to jump
            // Check if key was pressed down, the key is space or up arrow
            else if (event.key.key == SDLK_SPACE || event.key.key == SDLK_UP) {
                    player.jump();
            }
        }

        // If user closes window avoid processing and drawing an extra frame
        if (!running) break;

        // Check if jump keys currently held down
        const bool* keys = SDL_GetKeyboardState(nullptr);
        const bool jumpHeld = keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_UP];

        // Calculate how much time physics needs to advance
        Uint64 currentTime = SDL_GetTicksNS();
        double elapsed = static_cast<double>(currentTime - previousTime)/1'000'000'000.0;
        previousTime = currentTime;

        // Limit how much catch up work is done to account for if a window was paused
        if (elapsed > 0.25) elapsed = 0.25;

        // Increment how much catch up time
        accumulator += elapsed;

        // Update physics
        while (accumulator >= TPS) {
            // Move the player and detect landing first.
            player.update(dt, floorRect.y);

            // If held, jump as soon as grounded.
            if (jumpHeld) {
                player.jump();
            }

            accumulator -= TPS;
        }


        // Update rectangle position
        playerRect.y = player.getY();

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

        // Measure time elapsed in the frame
        Uint64 frameWorkNS = SDL_GetTicksNS() - frameStart;
        
        // Wait for the unused portion of how long the frame lasts
        if (frameWorkNS < targetFrameNS) {
            SDL_DelayNS(targetFrameNS - frameWorkNS);
        }
    }

    // Close the window, renderer, stuff like that
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
