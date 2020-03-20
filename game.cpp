//
//  game.cpp
//  baller
//
//  Created by martin on 2020-03-12.
//

#include "game.h"

//Basic game contructor
game::game(int width, int height)
{
    //Create opengl window
    window = SDL_CreateWindow(NULL, 0, 0, width, height, SDL_WINDOW_OPENGL);
    //Create renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
}

int game::random(int min, int max)
{
    return min + rand() % (max - min + 1);
}

void game::render(SDL_Renderer *renderer)
{

    SDL_Rect rect;
    Uint8 r, g, b;

    //Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //  Come up with a random rectangle
    rect.w = random(64, 128);
    rect.h = random(64, 128);
    rect.x = random(0, SCREEN_WIDTH);
    rect.y = random(0, SCREEN_HEIGHT);

    // Come up with a random color
    r = random(50, 255);
    g = random(50, 255);
    b = random(50, 255);
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // Fill the rectangle in the color
    SDL_RenderFillRect(renderer, &rect);

    // update screen
    SDL_RenderPresent(renderer);
}

void game::run()
{
    int done = 0;
    //Game loop
    while (!done)
    {
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = 1;
            }
        }
        render(renderer);
        SDL_Delay(1);
    }
    SDL_Log( "Failed to load media!\n" );
    // shutdown SDL
    SDL_Quit();
}
