//
// Created by Kamil Walkowicz on 03/01/2018.
//

#ifndef ARCSHOOT_CLASSES_H_INCLUDED
#define ARCSHOOT_CLASSES_H_INCLUDED

#include <SDL2/SDL.h>


typedef struct Shape {
    float x;
    float y;
} Shape;

void Shape_ctor(Shape* self, float x, float y);


typedef struct Vector {
    float x;
    float y;
} Vector;

void Vector_ctor(Vector* self, float x, float y);


/* MShape - Movable Shape - shape with a velocity vector */
typedef struct MShape {
    Shape  super;
    Vector velocity;
    Vector velocityGoal;
} MShape;

void MShape_ctor(MShape* self, float x, float y);
void MShape_update(MShape* self, float td);


typedef struct Rect {
    Shape super;
    float  width;
    float  height;
    SDL_Texture* tex;
} Rect;

void Rect_ctor(Rect* self, float x, float y,
                float width, float height, SDL_Texture* tex);
void Rect_render(Rect* self, SDL_Renderer* ren);
void Rect_destroy(Rect* self);


typedef struct MRect {
    MShape super;
    float  width;
    float  height;
    SDL_Texture* tex;
} MRect;

void MRect_ctor(MRect* self, float x, float y,
                float width, float height, SDL_Texture* tex);
void MRect_render(MRect* self, SDL_Renderer* ren);
void MRect_destroy(MRect* self);


typedef struct Player {
    MRect super;
    SDL_Texture* texUp;
    SDL_Texture* texNorm;
    SDL_Texture* texDown;
} Player;

void Player_ctor(Player* self, SDL_Renderer* ren);
void Player_controls(Player* self, SDL_Keycode key, uint keyDown);
void Player_destroy(Player* self);


typedef struct Background {
    MRect super;
    uint tiles_x;
    uint tiles_y;
    uint size;
    Rect* array;
} Background;

void Background_ctor(Background* self, float pos, SDL_Renderer* ren);
void Background_render(Background* self, SDL_Renderer* ren);
void Background_update(Background* self, float win_velocity_goal,
                       float td, SDL_Renderer* ren);
void Background_destroy(Background* self);

#endif //ARCSHOOT_CLASSES_H
