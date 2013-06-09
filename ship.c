#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include "ship.h"
#include "sprite.h"
#include "bmp.h"
#include "render.h"
#include "vl_array.h"
#include "matrix.h"

typedef struct Bullet {
    int x;
    int y;
    int w;
    int h;
    float vx;
    float vy;
} Bullet;

typedef struct Particle {
   Vector *pos;
   Vector *v;
   int life;
} Particle;

struct Ship {
    Vector* pos;
    Vector* v;
    float dv;
    int rot;
    float vrot; //current rotation speed
    float arot; //acceleration of rotation speed
    vl_array *bullets;
    vl_array *particles;
    Sprite sprite;
    Sprite rot_buffer;
    SDL_Surface *rainbow;
};

typedef struct Keys {
    char left;
    char right;
    char up;
    char down;
    char space;
} Keys;

Keys keys;

void reset_keys(){
    keys.left = 0;
    keys.right = 0;
    keys.up = 0;
    keys.down = 0;
    keys.space = 0;
}

Sprite *ship_sprite = 0;

Ship* get_ship(int x, int y)
{
    SDL_Surface *fb = SDL_GetVideoSurface();
    int bpp = fb->pitch / fb->w;
    Ship* ship = malloc(sizeof(Ship));
    ship->pos = malloc(sizeof(Vector));
    set_vector(ship->pos, x, y);
    ship->v = malloc(sizeof(Vector));
    set_vector(ship->v, 0, 0);
    ship->dv = 2;
    ship->rot = 0;
    ship->vrot = 0;
    ship->arot = 0.5;
    ship->bullets = new_array(sizeof (Bullet));
    ship->particles = new_array(sizeof(Particle));

    if (!ship_sprite){
        bmp_file *bmp = read_bmp("Sprites/ship.bmp");
        ship_sprite = malloc(sizeof(Sprite));
        ship_sprite->pixels = convert_bmp_to_pixels(bmp, 4);
        ship_sprite->w = get_bmp_width(bmp);
        ship_sprite->h = get_bmp_height(bmp);
        ship_sprite->bpp = 4;//get_bmp_bits_per_pixel(bmp);
        free_bmp(bmp);
    }

    ship->sprite = *ship_sprite;

    int rrad = ship->sprite.w/sin(45); //rotation buffer width and height
    Sprite rotBuffer = {malloc(sizeof(int) * rrad * rrad), rrad, rrad, bpp};
    memset(rotBuffer.pixels, 0, sizeof(int) * rrad * rrad);
    rotate_sprite(ship->sprite, rotBuffer, 0);
    ship->rot_buffer = rotBuffer;
    ship->rainbow = SDL_CreateRGBSurface(SDL_SWSURFACE, fb->w, fb->h, 8*fb->pitch/fb->w, 0, 0, 0, 0);
    int bytes = fb->h * fb->pitch;
    memset(ship->rainbow->pixels, 0, bytes);
    reset_keys();
    return ship;
}

void handle_ship_events(Ship *s, int eventType, int key)
{
    if (eventType == SDL_KEYDOWN){
        switch(key){
            case SDLK_UP:
                keys.up = 1;
                break;
            case SDLK_DOWN:
                keys.down = 1;
                break;
            case SDLK_LEFT:
                keys.left = 1;
                break;
            case SDLK_RIGHT:
                keys.right = 1;
                break;
            case SDLK_SPACE:
                keys.space = 1;
                break;
        }
    } else if (eventType == SDL_KEYUP){
        switch(key){
            case SDLK_UP:
                keys.up = 0;
                break;
            case SDLK_DOWN:
                keys.down = 0;
                break;
            case SDLK_LEFT:
                keys.left = 0;
                break;
            case SDLK_RIGHT:
                keys.right = 0;
                break;
        }
    }
}

void draw_ship(Ship *s)
{
    SDL_Surface *fb = SDL_GetVideoSurface();
    //draw rainbow
    //draw_rainbow(s->rainbow, s->x + s->sprite.w/2, s->y + s->sprite.h/2, (int)s->vx, (int)s->vy, 3);
    int ignore = 0x00000000;
    //fill_buffer_with_ignore (fb, s->rainbow, &ignore);
    //draw particles
    int i;
    for (i = 0; i < vl_array_length(s->particles); ++i){
        Particle *p = (Particle*) get_element(s->particles, i);
        int color = 0xffffff;
        draw_pixel((int)p->pos->x, (int)p->pos->y, fb, &color);
    }
    //draw ship
    draw_sprite_with_ignore(s->rot_buffer, fb, (int)s->pos->x, (int)s->pos->y, &ignore);
    //draw bullet
    for (i = 0; i < vl_array_length(s->bullets); ++i){
        Bullet *b = (Bullet*) get_element(s->bullets, i);
        int color = 0xffffff;
        draw_pixel(b->x, b->y, fb, &color);
        draw_pixel(b->x + 1, b->y, fb, &color);
        draw_pixel(b->x - 1, b->y, fb, &color);
        draw_pixel(b->x, b->y + 1, fb, &color);
        draw_pixel(b->x, b->y - 1, fb, &color);
    }
}

void add_smoke(Ship *s)
{
    int rot = s->rot + 180 + (rand() % 40) - 20;
    rot %=360;
    Vector *posVec = init_vector(s->pos->x + s->sprite.w / 2, s->pos->y + s->sprite.h / 2);
    Vector *rotVec = get_unit_vector(rot);
    scale_vector(rotVec, 0.5);
    Particle p = {posVec, rotVec, rand() % 50};
    add_element(s->particles, &p);
    //free(posVec);
    //free(rotVec);
}

void update_ship(Ship *s)
{
    if (keys.left){
        if (s->vrot < 7)
            s->vrot += s->arot;
    }
    if (keys.right){
        if (s->vrot > -7)
            s->vrot -= s->arot;
    }
    if (keys.space){
        fire_bullet(s);
        keys.space = 0;
    }
    if (keys.up){
        Vector *v = get_unit_vector(s->rot);
        s->v->x += v->x / 10;
        s->v->y += v->y / 10;
        free(v);
        add_smoke(s);
    }
    //update ship
    if (s->vrot){
        s->vrot *= 0.95;
        s->rot += s->vrot;
        s->rot %= 360;
        if (s->rot < 0) s->rot +=360;
        rotate_sprite(s->sprite, s->rot_buffer, s->rot);
    }
    s->v->x *= 0.95;
    s->v->y *= 0.95;
    s->pos->x += s->v->x;
    s->pos->y += s->v->y;
    if (s->pos->x < 0) s->pos->x = 0;
    if (s->pos->x + s->sprite.w > s->rainbow->w) s->pos->x = s->rainbow->w - s->sprite.w;
    if (s->pos->y < 0) s->pos->y = 0;
    if (s->pos->y + s->sprite.h > s->rainbow->h) s->pos->y = s->rainbow->h - s->sprite.h;

    //update bullets
    int i;
    for (i = 0; i < vl_array_length(s->bullets); ++i){
        Bullet* b = ((Bullet*) get_element(s->bullets, i));
        b->x += (int)b->vx;
        b->y += (int)b->vy;
        if (point_offscreen (s->rainbow, b->x, b->y) || point_offscreen(s->rainbow, b->x + b->w, b->y + b->h)){
            remove_element(s->bullets, b);
            --i;
        }
    }
    // update particles
    for (i = 0; i < vl_array_length(s->particles); ++i){
        Particle *p = (Particle*) get_element(s->particles, i);
        --(p->life);
        p->pos->x += p->v->x;
        p->pos->y += p->v->y;
        if (p->life < 0 || point_offscreen(s->rainbow, p->pos->x, p->pos->y)) {
	    free(p->pos);
	    free(p->v);
            remove_element(s->particles, p);
            --i;
            continue;
        }
    }
}

void fire_bullet(Ship* s)
{
    Bullet b;
    b.x = s->pos->x + s->sprite.w/2;
    b.y = s->pos->y + s->sprite.h/2;
    b.w = 5;
    b.h = 5;
    Vector *v = get_unit_vector(s->rot);
    b.vx = v->x * 5;
    b.vy = v->y * 5;
    add_element(s->bullets, &b);
}

int get_ship_vx(Ship *s)
{
    int ret = 0;
    if (s->v->x > 0) ret = 1;
    if (s->v->x < 0) ret = -1;
    return ret;
}

int get_ship_vy(Ship *s)
{
    int ret = 0;
    if (s->v->y > 0) ret = 1;
    if (s->v->y < 0) ret = -1;
    return ret;
}

int get_ship_rotation(Ship *s)
{
    return s->rot;
}

int get_ship_vr(Ship *s){
    int ret = 0;
    if (s->vrot > 0) ret = 1;
    if (s->vrot < 0) ret = -1;
    return ret;
}

#undef R
#undef G
#undef B
