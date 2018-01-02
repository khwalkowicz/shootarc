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

typedef struct Player {
    Rect super;
    double acc; //acceleration
    SDL_Texture* tex_up;
    SDL_Texture* tex_norm;
    SDL_Texture* tex_down;
} Player;

typedef struct Background {
    uint size;
    uint tile_h;
    uint tile_w;
    uint assets_available;
    uint tiles_y;
    uint tiles_x;
    char* path;
    double move;
    double speed; //acceleration
    Rect* array;
} Background;

void Rect_render(Rect obj, SDL_Renderer* ren);
void Rect_destroy(Rect obj);

Player Player_init(SDL_Renderer* ren);
void Player_move(Player *player, Background* bg, SDL_Keycode key,
                 uint keyDown, double timeDelta);
void Player_destroy(Player obj);

Background Background_init(SDL_Renderer* ren);
void Background_scroll(Background* obj, SDL_Renderer* ren, float timeDelta);
void Background_render(Background obj, SDL_Renderer* ren);
void Background_destroy(Background obj);

#endif //GAME_CLASSES_H
