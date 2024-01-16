#pragma once
#include <SDL.h>

class Food {
public:
    // Constructor
    Food(SDL_Renderer* renderer);

    void render();

    // Fucntion to change the food's postion whenver it is eaten
    void respawn();

    //Getters
    int getX();
    int getY();

private:
    SDL_Renderer* renderer;
    SDL_Rect foodRect;
    int size;
};
