#include <SDL/SDL.h>

#include "render.h"
#include "ship.h"
#include "starfield.h"
#include "trigtable.h"


void update();
void draw();
int handle_events();

Ship* ship;
Starfield *stars;

extern int RUNNING;

int run_game ()
{
    RUNNING = 1;
    ship = get_ship(100,100);
    stars = get_starfield();

    while (RUNNING){
	    handle_events();
	    update();
	    draw();

	    SDL_Flip(SDL_GetVideoSurface());
	    SDL_Delay (16);
	}
	return 0;
}



void update()
{
    update_starfield(stars, get_ship_rotation(ship));
    update_ship(ship);
}

void draw()
{
    clear_buffer(SDL_GetVideoSurface());
    draw_starfield(stars);
    draw_ship(ship);
}

int handle_events()
{
    SDL_Event event;
    int ret = 0;
    while( SDL_PollEvent( &event ) ){
            //If the user has Xed out the window
        if( event.type == SDL_QUIT ){
            RUNNING = 0;
            //Quit the program
            ret = 1;
        } else {
            handle_ship_events(ship, event.type, event.key.keysym.sym);
        }
    }
	return ret;
}

