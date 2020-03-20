//
//  game.h
//  baller
//
//  Created by martin on 2020-03-12.
//
#include "SDL.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

class game
{
    public:
    //Varibles
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    //Constructor
    game(int width, int height);
    //Functions
    void run();
    void render(SDL_Renderer *renderer);
    int random(int min, int max);
};
