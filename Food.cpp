#include "food.h"
#include <cstdlib>
#include <ctime>

Food::Food(SDL_Renderer* renderer){
    this->renderer = renderer;
    size = 20;
    srand(std::time(0));
    respawn();
}

void Food::render() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color
    SDL_RenderFillRect(renderer, &foodRect);
}

void Food::respawn() {
    // Respawn the food at a random location not occupied by the snake
    foodRect.x = (rand() % (620 / size)) * size;
    foodRect.y = (rand() % (460 / size)) * size;
    foodRect.w = size;
    foodRect.h = size;
}

int Food::getX() {
    return foodRect.x;
}

int Food::getY() {
    return foodRect.y;
}