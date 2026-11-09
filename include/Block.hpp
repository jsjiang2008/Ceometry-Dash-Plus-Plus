#pragma once
#include <SDL3/SDL.h>

class Block {
private:
    // Position and dimensions in the level, before scrolling.
    SDL_FRect bounds;

public:
    Block(float x, float y, float width, float height) 
        :bounds{x, y, width, height} {
    }

    SDL_FRect getVisualRect(float scrollX) const {
        SDL_FRect rect = bounds;
        rect.x -= scrollX;
        return rect;
    }

    SDL_FRect getHitbox(float scrollX) const {
        // The entire block is solid.
        return getVisualRect(scrollX);
    }

    void draw(SDL_Renderer* renderer, float scrollX, bool showHitbox = false) const {
        const SDL_FRect rect = getVisualRect(scrollX);

        // Draw a blue block with a white border.
        SDL_SetRenderDrawColor(renderer, 60, 100, 150, 255);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &rect);

        if (showHitbox) {
            const SDL_FRect box = getHitbox(scrollX);
            // The red hitbox outline covers the block's border.
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderRect(renderer, &box);
        }
    }
};
