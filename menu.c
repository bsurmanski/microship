#include <SDL/SDL.h>
#include "render.h"

int RUNNING = 1;
int selection = 0;

void handle_menu_events();
void update_menu();
void draw_menu();

int run_menu()
{
    while(RUNNING){
        handle_menu_events();
        update_menu();
        draw_menu();

        SDL_Flip(SDL_GetVideoSurface());
        SDL_Delay(16);
    }
    return selection;
}

void handle_menu_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            RUNNING = 0;
        }
    }
}

void update_menu()
{

}

void draw_menu()
{
    clear_buffer(SDL_GetVideoSurface());
    draw_text("START", SDL_GetVideoSurface(), 100, 100);
    draw_text("EXIT", SDL_GetVideoSurface(), 100, 110);
}
