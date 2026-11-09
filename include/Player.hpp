// Only compile this once
#pragma once

class Player {
private:
    // Private: only accessible within this class
    float yPosition;
    float yVelocity;
    float playerHeight;
    bool grounded;
    

public:
    // Functions to be implemented
    Player(float initialY, float h);
    void jump();
    void update(float dt, float floorY);
    float getY() const {
        return yPosition; 
    }
    void landOn(float surfaceY);
    bool isFalling() const {
        return yVelocity >= 0.0f;
    }
};
