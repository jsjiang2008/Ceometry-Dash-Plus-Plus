#pragma once
#include <SDL3/SDL.h>

class Spike {
private:
    SDL_FRect bounds;

public:
    Spike(float x, float y, float width, float height) 
        :bounds{x, y, width, height} {
    }

    SDL_FRect getVisualRect(float scrollX) const {
        SDL_FRect rect = bounds;
        rect.x -= scrollX;
        return rect;
    }

    SDL_FRect getHitbox(float scrollX) const {
        SDL_FRect rect = getVisualRect(scrollX);

        // Create a hitbox in the center
        return {
            rect.x + rect.w * 0.30f,
            rect.y + rect.h * 0.50f,
            rect.w * 0.40f,
            rect.h * 0.50f
        };
    }

    void draw(SDL_Renderer* renderer, float scrollX, bool showHitbox = false) const {
        const SDL_FRect rect = getVisualRect(scrollX);

        // Bottom left, tip, bottom right, bottom left to form a spike
        const SDL_FPoint points[]{
            {rect.x,                rect.y + rect.h},
            {rect.x + rect.w * 0.5f, rect.y},
            {rect.x + rect.w,        rect.y + rect.h},
            {rect.x,                rect.y + rect.h}
        };

        // Draw lines from the points to each other
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderLines(renderer, points, 4);

        if (showHitbox) {
            const SDL_FRect box = getHitbox(scrollX);
            // Red hitbox
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderRect(renderer, &box);
        }
    }
};