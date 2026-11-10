#include <SDL3/SDL.h>
#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include "Player.hpp"
#include "Level.hpp"

int main(int argc, char* argv[]) {
    if (argc > 2 || (argc == 2 && std::string_view(argv[1]) == "--help")) {
        std::cout << "Usage: ceometry-dash [path/to/level.level]\n";
        return argc > 2 ? 1 : 0;
    }

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

    // Define default size of one block
    constexpr float blockSize = 40.0f;

    // Create two rectangles, one for the ground, one for the floor
    // The parameters are as follows {distance from left of screen, distance from top of screen, width, height}
    SDL_FRect floorRect{0.0f, 500.0f, 800.0f, 100.0f};
    SDL_FRect playerRect{150.0f, floorRect.y - blockSize, blockSize, blockSize};

    // Create objects

    // Player
    Player player{playerRect.y, playerRect.h};

    // Spikes
    float scrollX = 0.0f;
    constexpr float scrollSpeed = 10.386f * blockSize; // 10.386 is measured block speed in original game

    // Load the chosen file, or the default level beside the executable.
    Level level;
    try {
        std::filesystem::path levelPath;
        if (argc == 2) {
            levelPath = argv[1];
        } else {
            const char* basePath = SDL_GetBasePath();
            if (!basePath) {
                throw std::runtime_error(SDL_GetError());
            }
            levelPath = std::filesystem::path(basePath) / "levels" / "first.level";
        }
        level = Level::loadFromFile(levelPath, blockSize, floorRect.y);
    } catch (const std::exception& error) {
        std::cerr << "Level loading failed: " << error.what() << '\n';
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    const auto& spikes = level.getSpikes();
    const auto& blocks = level.getBlocks();
    
    // Run the program and get events that happen
    bool running = true;
    SDL_Event event;

    // Physics advance in steps of 1/240 of a second (Ticks per second)
    // 2.2 Geometry dash uses 240 TPS
    // This way movement stays consistent when rendering speed changes
    constexpr double TPS = 1.0 / 240.0;
    double accumulator = 0.0;
    Uint64 previousTime = SDL_GetTicksNS();
    constexpr float dt = static_cast<float>(TPS);

    // Define frame target
    constexpr Uint64 targetFPS = 60;
    constexpr Uint64 targetFrameNS = 1'000'000'000ULL / targetFPS;

    // Restart method variable/function
    // Use lambda instead of method to access 
    // local variables without passing them
    const auto restartLevel = [&]() {
        player = Player{
            floorRect.y - playerRect.h,
            playerRect.h
        };

        playerRect.y = player.getY();
        scrollX = 0.0f;

        accumulator = 0.0;
        previousTime = SDL_GetTicksNS();
    };

    while (running) {
        Uint64 frameStart = SDL_GetTicksNS();

        // This while loop checks the events that happened since the last frame
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                // Check if user wants to quit
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN && 
                        event.key.key == SDLK_R && !event.key.repeat) {
                // If 'R' is pressed restart level
                restartLevel();
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
            // Remember previous surface
            const float previousBottom = player.getY() + playerRect.h;

            // Move the player and detect landing first.
            player.update(dt, floorRect.y);

            // Scroll in the level
            scrollX += scrollSpeed * dt;

            // Use player position to check for death
            playerRect.y = player.getY();
            const float currentBottom = playerRect.y + playerRect.h;

            // Find the highest block top crossed during this step.
            float landingY = floorRect.y;

            if (player.isFalling()) {
                for (const Block& block : blocks) {
                    const SDL_FRect box = block.getHitbox(scrollX);

                    const bool overlapsHorizontally =
                        playerRect.x < box.x + box.w &&
                        playerRect.x + playerRect.w > box.x;

                    const bool crossedTop =
                        previousBottom <= box.y &&
                        currentBottom >= box.y;
                    
                    if (overlapsHorizontally && crossedTop && box.y < landingY) {
                        landingY = box.y;
                    }
                }
            }

                if (landingY < floorRect.y) {
                    player.landOn(landingY);
                    playerRect.y = player.getY();
                }


            // Bool to track if death happened
            bool died = false;


            // See if crash into block
            for (const Block& block : blocks) {
                const SDL_FRect box = block.getHitbox(scrollX);

                const bool overlaps =
                    playerRect.x < box.x + box.w &&
                    playerRect.x + playerRect.w > box.x &&
                    playerRect.y < box.y + box.h &&
                    playerRect.y + playerRect.h > box.y;

                if (overlaps) {
                    died = true;
                    break;
                }
            }

            // for loop: const to not change objects, & to access without copying
            for (const Spike& spike : spikes) {
                if (died) break;

                const SDL_FRect hitbox = spike.getHitbox(scrollX);
                
                // Set died to true
                if (SDL_HasRectIntersectionFloat(&playerRect, &hitbox)) {
                    died = true;
                    break;
                }
            }

            if (died) {
                // Restart and leave before subtracting TPS
                restartLevel();
                break;
            }
            
            // If held, jump as soon as grounded.
            if (jumpHeld) {
                player.jump();
            }

            accumulator -= TPS;
        }


        // Update rectangle position
        playerRect.y = player.getY();

        // Paint the background dark blue
        // Color values are red, green, blue, alpha
        // 255 alpha is fully solid, no transparency
        SDL_SetRenderDrawColor(renderer, 20, 25, 40, 255);

        // Wipe screen clean
        SDL_RenderClear(renderer);

        // Draw the floor gray
        SDL_SetRenderDrawColor(renderer, 90, 95, 110, 255);
        SDL_RenderFillRect(renderer, &floorRect);

        // Draw spikes
        for (const Spike& spike : spikes) {
            spike.draw(renderer, scrollX, true);
        }

        for (const Block& block : blocks) {
            block.draw(renderer, scrollX, true);
        }

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
