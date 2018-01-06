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


typedef struct Movable {
    Vector velocity;
    Vector velocityGoal;
} Movable;

void Movable_ctor(Movable* self, float velX, float velY,
                  float velGoalX, float velGoalY);
void Movable_update(Movable* self, float td);

typedef struct MShape {
    Shape  super;
    Movable vectors;
} MShape;

void MShape_ctor(MShape* self, float x, float y);
void MShape_update(MShape* self, float td);


typedef struct Rect {
    Shape super;
    float width;
    float height;
    SDL_Texture* tex;
} Rect;

void Rect_ctor(Rect* self, float x, float y,
                float width, float height, SDL_Texture* tex);
void Rect_render(Rect* self, SDL_Renderer* ren);
uint Rect_checkWallsX(Rect* self);
uint Rect_checkWallsY(Rect* self);
void Rect_destroy(Rect* self);


typedef struct RectArr {
    uint size;
    uint idx;
    Rect** arr;
} RectArr;

void RectArr_ctor(RectArr* self);
uint RectArr_add(RectArr* self, Rect* rectPtr);
void RectArr_sort(RectArr* self, char towards);
void RectArr_del(RectArr* self, Rect* rectPtr);
RectArr RectArr_checkCollision(RectArr* fg, Rect* obj);
void RectArr_destroy(RectArr* self);


typedef struct MRect {
    Rect super;
    Movable vectors;
} MRect;

void MRect_ctor(MRect* self, float x, float y,
                float width, float height, SDL_Texture* tex);
void MRect_update(MRect* self, float td, uint checkCollision, RectArr* fg);


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
    uint  tiles_x;
    uint  tiles_y;
    uint  size;
    Rect* array;
} Background;

void Background_ctor(Background* self, float pos, SDL_Renderer* ren);
void Background_render(Background* self, SDL_Renderer* ren);
void Background_update(Background* self, float win_velocity_goal,
                       float td, SDL_Renderer* ren);
void Background_destroy(Background* self);


#endif //ARCSHOOT_CLASSES_H
