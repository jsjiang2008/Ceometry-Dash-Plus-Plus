// Only compile this once
#pragma once

// The gamemodes the player can use.
enum class PlayerMode {
    Cube,
    Ship
};

class Player {
private:
    // Private: only accessible within this class
    float yPosition;
    float yVelocity;
    float playerHeight;
    bool grounded;

    // Players default to cube mode; main.cpp can choose another starting mode.
    PlayerMode mode = PlayerMode::Cube;
    

public:
    // Functions to be implemented
    Player(float initialY, float h);

    // Read or change the current mode; movement rules are handled in update().
    PlayerMode getMode() const {
        return mode;
    }
    void setMode(PlayerMode newMode) {
        mode = newMode;
    }

    void jump();
    // Ships use held input for continuous thrust; cubes jump separately after landing.
    void update(float dt, float floorY, bool jumpHeld);
    float getY() const {
        return yPosition; 
    }
    void landOn(float surfaceY);
    bool isFalling() const {
        return yVelocity >= 0.0f;
    }
};
