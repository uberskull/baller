/*
 *  baller
 *  written by Martin Borjesson
 *  proprietary license
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include "game.h"


int main(int argc, char *argv[])
{
    game *theGame = new game(SCREEN_WIDTH, SCREEN_HEIGHT);
    theGame->run();

    return 0;
}
