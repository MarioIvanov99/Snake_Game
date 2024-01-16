#include "snake.h"

Snake::Snake(SDL_Renderer* renderer, int initialLength) {
    this->renderer = renderer;
    currentDirection = 1; // 1 represents right, 2 represents up, 3 represents left, 4 represents down
    size = 20;
    for (int i = 0; i < initialLength; i++) {
        segments.push_back({ 320 - i * size, 240, currentDirection }); // Put the snake in the middle of the window
    }

    // Separate delay for fps and snake
    // Goal is to slow the snake down, while keeping FPS up, since otherwise rendering shorter snakes' movement is choppy and there are control issues
    moveDelay = 150; 
    lastMoveTime = SDL_GetTicks();

    // Flag to make sure snake can't set a new direction while moving
    isMoving = false;
}

void Snake::render() {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Set color to green

    for (SnakeSegment& segment : segments) {
        SDL_Rect snakeRect = { segment.x, segment.y, size, size };
        SDL_RenderFillRect(renderer, &snakeRect);  // Render a filled rectangle for each segment
    }
}

void Snake::changeDirection(int newDirection) {

    if (!isMoving) {
        // Check if the new direction is opposite to the current direction
        if ((newDirection == 1 && currentDirection == 3) ||
            (newDirection == 3 && currentDirection == 1) ||
            (newDirection == 2 && currentDirection == 4) ||
            (newDirection == 4 && currentDirection == 2)) {
            // Ignore the input if it's the opposite direction
            return;
        }
        currentDirection = newDirection;
        isMoving = true;
    }
}

void Snake::move() {
    // Move the snake based on the current direction

    // Control the speed by introducing a delay between movements
    Uint32 currentFrameTime = SDL_GetTicks();
    Uint32 frameTimeDifference = currentFrameTime - lastMoveTime;

    if (frameTimeDifference >= moveDelay) {
        // Move the head
        SnakeSegment newHead = { segments.front().x, segments.front().y, currentDirection };
        switch (currentDirection) {
        case 1: // Right
            newHead.x += size;
            break;
        case 2: // Up
            newHead.y -= size;
            break;
        case 3: // Left
            newHead.x -= size;
            break;
        case 4: // Down
            newHead.y += size;
            break;
        default:
            break;
        }

        // Insert the new head at the front
        segments.push_front(newHead);

        // Remove the tail (if needed)
        segments.pop_back();

        lastMoveTime = currentFrameTime;  // Update the last move time
        isMoving = false;
    }

}

int Snake::getX() {
    return segments.front().x;
}

int Snake::getY() {
    return segments.front().y;
}

void Snake::grow() {
    // Add a new segment to the end of the snake
    SnakeSegment tail = segments.back();  // Get the current tail segment
    switch (tail.localDirection) {
    case 1: 
        tail.x -= size;
        break;
    case 2: 
        tail.y += size;
        break;
    case 3: 
        tail.x += size;
        break;
    case 4: 
        tail.y -= size;
        break;
    default:
        break;
    }

    // Add the new tail segment
    segments.push_back(tail);
}

bool Snake::checkUnitCollision(int x, int y, int start) {

    // Used to check collision with self or food
    for (int i = start; i < segments.size(); i++) {
        if (x == segments[i].x && y == segments[i].y) {
            return true;
        }
    }
    
    return false;

}

bool Snake::checkSideCollision() {
    // Check if the head has gone out of bounds
    if (segments.front().x < 0 || segments.front().x >= 640 ||
        segments.front().y < 0 || segments.front().y >= 480) {
        return true;
    }
    
    return false;
}

void Snake::reset() {
    // Clear existing segments
    segments.clear();

    // Reinitialize the snake with the initial length
    currentDirection = 1;
    for (int i = 0; i < 3; i++) {
        segments.push_back({ 320 - i * size, 240, currentDirection });
    }

}