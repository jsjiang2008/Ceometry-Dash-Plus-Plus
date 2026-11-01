#include "Player.hpp"

// Y increases as you go down so gravity positive, jump velocity negative
constexpr float gravity = 1800.0f;      // Pixels per second squared
constexpr float jumpVelocity = -650.0f; // Pixels per second 


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
    yVelocity += gravity * dt;
    yPosition += yVelocity * dt;
    if (yPosition + playerHeight >= floorY) {
        yPosition = floorY - playerHeight;
        yVelocity = 0.0f;
        grounded = true;
    }
}

