#include <SDL.h>
#include <SDL_ttf.h>
#include "snake.h"
#include "food.h"
#include <iostream>
#include <sstream>

// Function to render text using SDL_ttf
SDL_Texture* renderText(const std::string& text, TTF_Font* font, SDL_Renderer* renderer) {
    // Create a surface from the text
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255, 255 });
    if (!surface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    // Create a texture from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
    }

    // Free the surface as it's no longer needed
    SDL_FreeSurface(surface);

    return texture;
}

bool checkCollision(int snakeX, int snakeY, int foodX, int foodY) {
    // Check if the head of the snake collides with the food
    return (snakeX==foodX && snakeY == foodY);
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Handle initialization error
        return 1;
    }


    // Create a window
    SDL_Window* window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        // Handle window creation error
        std::cerr << "SDL window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "SDL renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("OpenSans-Regular.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Score
    int score = 0; 
    std::stringstream scoreText;
    scoreText << "Score: " << score;

    // Create Snake and Food
    Snake snake(renderer);
    Food food(renderer);
    
    // Quit condition for window
    bool quit = false;

    // Used to set fps
    Uint32 lastFrameTime = SDL_GetTicks();

    // Used to make sure inputs don't get consumed
    int lastDirection = 1;

    // Start and Game Over flags
    bool gameOver = false;
    bool start = true;
        
    while (!quit) {

        // Handle Input
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    lastDirection = 2; // Up
                    break;
                case SDLK_DOWN:
                    lastDirection = 4; // Down
                    break;
                case SDLK_LEFT:
                    lastDirection = 3; // Left
                    break;
                case SDLK_RIGHT:
                    lastDirection = 1; // Right
                    break;
                case SDLK_r:
                    if (start) { // Only at start of game
                        start = false;
                    }
                    else if (gameOver) {

                        // Reset game state
                        snake.reset();
                        food.respawn();
                        score = 0;
                        gameOver = false;
                        lastDirection = 1;
                        // Clear the previous score text
                        scoreText.str("");
                        scoreText << "Score: " << score;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        if (!gameOver) { // Game logic
            
            snake.changeDirection(lastDirection);

            if (!snake.checkUnitCollision(snake.getX(), snake.getY(), 1) && !snake.checkSideCollision()) { // Fail condition
                // Don't run while on the start screen
                if (!start) {
                    snake.move();
                }
            }
            else {
                gameOver = true;
            }

            if (checkCollision(snake.getX(), snake.getY(), food.getX(), food.getY())) {

                // Code to handle scoring
                snake.grow();
                score++;
                scoreText.str(""); 
                scoreText << "Score: " << score;

                // Code to handle food respawn
                // It cannot spawn in a space occupied by the snake
                while (snake.checkUnitCollision(food.getX(), food.getY(), 0)) {
                    food.respawn();
                }

            }
        }

        // Render Scene
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);  // Clear the screen

        // Code to handle text rendering
        if (start) {

            // Render start screen text
            TTF_Font* largeFont = TTF_OpenFont("OpenSans-Regular.ttf", 40);
            SDL_Texture* startScreenTexture = renderText("Press 'R' to play snake", largeFont, renderer);

            int width, height;
            SDL_QueryTexture(startScreenTexture, nullptr, nullptr, &width, &height);

            // Put text in the center of the screen
            int startScreenX = (640 - width) / 2;
            int startScreenY = (480 - height) / 2;

            // Render text
            SDL_Rect startScreenRect = { startScreenX, startScreenY, width, height };
            SDL_RenderCopy(renderer, startScreenTexture, nullptr, &startScreenRect);
            SDL_DestroyTexture(startScreenTexture);
        }
        else if (!gameOver) {
            // Render the score text
            SDL_Texture* scoreTexture = renderText(scoreText.str(), font, renderer);

            // Render the snake and food
            food.render();
            snake.render();

            int width, height;

            // Render the score text at the top-left corner
            int x = 10;
            int y = 10;
        
            SDL_QueryTexture(scoreTexture, nullptr, nullptr, &width, &height);
            SDL_Rect dstRect = { x, y, width, height };
            SDL_RenderCopy(renderer, scoreTexture, nullptr, &dstRect);
            SDL_DestroyTexture(scoreTexture);
        }
        else {
            // Render "Game Over" text
            TTF_Font* largeFont = TTF_OpenFont("OpenSans-Regular.ttf", 40);
            SDL_Texture* gameOverTexture = renderText("Game Over", largeFont, renderer);

            int width, height;
            SDL_QueryTexture(gameOverTexture, nullptr, nullptr, &width, &height);

            int gameOverX = (640 - width) / 2;
            int gameOverY = (480 - height) / 2 - height - 20;


            SDL_Rect gameOverRect = { gameOverX, gameOverY, width, height };
            SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
           
            // Render the final score text 
            SDL_Texture* scoreTexture = renderText("Final Score: " + std::to_string(score), largeFont, renderer);
            
            int scoreX = (640 - width) / 2;
            int scoreY = (480 - height) / 2; 

            // Render the score text below the "Game Over" text
            SDL_Rect scoreRect = { scoreX, scoreY, width, height };
            SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);

            // Render "Press 'R' to restart" text
            SDL_Texture* restartTexture = renderText("Press 'R' to restart", font, renderer);

            // Calculate the position to center the restart text below the final score
            int restartX = (640 - width) / 2;
            int restartY = scoreY + height + 20;

            // Render the restart text below the final score
            SDL_Rect restartRect = { restartX, restartY, width, height };
            SDL_RenderCopy(renderer, restartTexture, nullptr, &restartRect);

            SDL_DestroyTexture(gameOverTexture);
            SDL_DestroyTexture(scoreTexture);
            SDL_DestroyTexture(restartTexture);
        }

        SDL_RenderPresent(renderer);  // Update the screen

        Uint32 currentFrameTime = SDL_GetTicks();
        Uint32 frameTimeDifference = currentFrameTime - lastFrameTime;

        if (frameTimeDifference < 16) {
            // Set fps to ~60
            SDL_Delay(16 - frameTimeDifference);
        }

        lastFrameTime = currentFrameTime;

   
    }

    TTF_CloseFont(font);
    TTF_Quit();

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "SDL initialization and window creation successful!" << std::endl;

    return 0;
}