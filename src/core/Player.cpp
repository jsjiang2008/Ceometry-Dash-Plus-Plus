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

// Update position and velocity every frame
void Player::update(float dt, float floorY) {
    // Cube has a max fall speed in the original game
    yVelocity = std::min(yVelocity + gravity * dt, maxFallSpeed);
    yPosition += yVelocity * dt;
    if (yPosition + playerHeight >= floorY) {
        yPosition = floorY - playerHeight;
        yVelocity = 0.0f;
        grounded = true;
    }
}

