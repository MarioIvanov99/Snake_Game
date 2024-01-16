#pragma once
#include <SDL.h>
#include <deque>

class Snake {
public:
    // Constructor for the snake object
    Snake(SDL_Renderer* renderer, int initialLength = 3);

    void render();

    // Function to handle changing the direction of the snake
    void changeDirection(int newDirection);

    // Function to update the snake's position
    void move();

    // Getters
    int getX();
    int getY();

    // Function to increase the size of the nake
    void grow();

    // Functions to check for collisions with sides or self
    bool checkUnitCollision(int x, int y, int start);
    bool checkSideCollision();

    // Function that resets the snake to starting position and length
    void reset();

private:
    
    // Position and the direction the segment was facing when it was created
    struct SnakeSegment {
        int x, y, localDirection;
    };

    // Deque of snake segments
    std::deque<SnakeSegment> segments;

    // Current direction of movement
    int currentDirection;

    // Pointer to the renderer
    SDL_Renderer* renderer;

    // Size of each segment
    int size;

    // Move time
    Uint32 lastMoveTime;

    // Delay for movement
    Uint32 moveDelay;

    // Flag for movement
    bool isMoving;
};