#include "Player.hpp"
#include <algorithm>

// Y increases as you go down so gravity positive, jump velocity negative
constexpr float gravity = 3779.7296f;
constexpr float jumpVelocity = -805.9536f;  
constexpr float maxFallSpeed = 1080.144f;

// Constructor
Player::Player(float initialY, float h) {
    yPosition = initialY;
    yVelocity = 0.0f;
    grounded = true; 
    playerHeight = h;
}

// Jump
void Player::jump() {
    if (grounded) {
        yVelocity = jumpVelocity;
        grounded = false;
    }
}

// Landing
void Player::landOn(float surfaceY) {
    yPosition = surfaceY - playerHeight;
    yVelocity = 0.0f;
    grounded = true;
}

// Update position and velocity every frame
void Player::update(float dt, float floorY) {
    // Recheck support each step, including after leaving a block
    grounded = false;

    // Cube has a max fall speed in the original game
    yVelocity = std::min(yVelocity + gravity * dt, maxFallSpeed);
    yPosition += yVelocity * dt;


    if (yPosition + playerHeight >= floorY) {
        landOn(floorY);
    }
}

