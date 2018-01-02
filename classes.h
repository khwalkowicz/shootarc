//
// Created by Kamil Walkowicz on 02/01/2018.
//

#ifndef GAME_CLASSES_H_INCLUDED
#define GAME_CLASSES_H_INCLUDED

#include <SDL2/SDL.h>

typedef struct Shape {
    uint x;
    uint y;
} Shape;

typedef struct Rect {
    Shape super;
    uint w;
    uint h;
    SDL_Texture* tex;
} Rect;

typedef struct Background {
    uint size;
    uint tile_h;
    uint tile_w;
    uint assets_available;
    uint tiles_y;
    uint tiles_x;
    char* path;
    double move;
    Rect* array;
} Background;

Rect Rect_initPlayer(SDL_Renderer* ren);
void Rect_render(Rect obj, SDL_Renderer* ren);
void Rect_destroy(Rect obj);

Background Background_init(SDL_Renderer* ren);
void Background_scroll(Background* obj, SDL_Renderer* ren, float timeDelta);
void Background_render(Background obj, SDL_Renderer* ren);
void Background_destroy(Background obj);

#endif //GAME_CLASSES_H
