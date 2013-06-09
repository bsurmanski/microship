#include <SDL/SDL.h>
#include "menu.h"
#include "game.h"

int screenx = 320, screeny = 240, screen_bit = 32;
SDL_Surface *buffer;

void init ()
{
	    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	    buffer = SDL_SetVideoMode( screenx, screeny, screen_bit, SDL_SWSURFACE );
	    SDL_WM_SetCaption( "Microship", NULL );
}


int main (int argc, char *argv[])
{
    init();
    //switch (run_menu()){

    //}
    run_game();
    SDL_Quit();
	return 0;
}
