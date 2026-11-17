#include "Player.hpp"
#include <algorithm>

// Y increases as you go down so gravity positive, jump velocity negative
constexpr float gravity = 3779.7296f;
constexpr float jumpVelocity = -805.9536f;  
constexpr float maxFallSpeed = 1080.144f;

// Adjustable prototype ship values, not measured Geometry Dash physics.
constexpr float shipGravity = 900.0f;   // Downward acceleration in pixels/second squared.
constexpr float shipThrust = 1800.0f;   // Upward acceleration while the button is held.
constexpr float shipMaxSpeed = 450.0f;  // Vertical speed limit in pixels/second.

// Constructor
Player::Player(float initialY, float h) {
    yPosition = initialY;
    yVelocity = 0.0f;
    grounded = true; 
    playerHeight = h;
}

// Only cubes jump; ships use continuous thrust in update().
void Player::jump() {
    if (mode == PlayerMode::Cube && grounded) {
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
void Player::update(float dt, float floorY, bool jumpHeld) {
    // Recheck support each step, including after leaving a block
    grounded = false;

    if (mode == PlayerMode::Ship) {
        // Holding adds lift against gravity; releasing leaves only gravity.
        const float acceleration = shipGravity - (jumpHeld ? shipThrust : 0.0f);
        yVelocity = std::clamp(
            yVelocity + acceleration * dt, -shipMaxSpeed, shipMaxSpeed
        );
    } else {
        // Keep the existing cube gravity and maximum fall speed.
        yVelocity = std::min(yVelocity + gravity * dt, maxFallSpeed);
    }

    yPosition += yVelocity * dt;

    // Use the top of the screen as the ship's ceiling until flight corridors exist.
    if (mode == PlayerMode::Ship && yPosition < 0.0f) {
        yPosition = 0.0f;
        yVelocity = std::max(yVelocity, 0.0f);
    }

    if (yPosition + playerHeight >= floorY) {
        landOn(floorY);
    }
}

