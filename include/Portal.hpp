#pragma once

#include <SDL3/SDL.h>

#include "Player.hpp"

class Portal {
private:
    SDL_FRect bounds;
    PlayerMode targetMode;

public:
    Portal(float x, float y, float width, float height, PlayerMode mode)
        : bounds{x, y, width, height}, targetMode{mode} {
    }

    SDL_FRect getVisualRect(float scrollX) const {
        SDL_FRect rect = bounds;
        rect.x -= scrollX;
        return rect;
    }

    SDL_FRect getHitbox(float scrollX) const {
        // Entering any part of the gate activates it; the gate is not solid.
        return getVisualRect(scrollX);
    }

    PlayerMode getTargetMode() const {
        return targetMode;
    }

    void draw(SDL_Renderer* renderer, float scrollX, bool activated) const {
        const SDL_FRect rect = getVisualRect(scrollX);

        // Green gates select cube mode; blue gates select ship mode.
        SDL_Color color = targetMode == PlayerMode::Cube
            ? SDL_Color{80, 220, 120, 255}
            : SDL_Color{80, 180, 255, 255};
        if (activated) {
            color = SDL_Color{90, 95, 110, 255}; // Used gates stay dim until restart.
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderRect(renderer, &rect);

        // A second outline makes the portal look different from a solid block.
        const SDL_FRect inner{
            rect.x + rect.w * 0.12f, rect.y + rect.h * 0.02f,
            rect.w * 0.76f, rect.h * 0.96f
        };
        SDL_RenderRect(renderer, &inner);
        SDL_RenderDebugText(renderer, rect.x + rect.w * 0.5f - 16.0f, rect.y + 8.0f,
                            targetMode == PlayerMode::Cube ? "CUBE" : "SHIP");
    }
};
