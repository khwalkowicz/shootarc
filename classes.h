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
    char* type;
    float width;
    float height;
    SDL_Texture* tex;
} Rect;

void Rect_ctor(Rect* self, char* type, float x, float y,
                float width, float height, SDL_Texture* tex);
void Rect_render(Rect* self, SDL_Renderer* ren);
uint Rect_checkWallsX(Rect* self);
uint Rect_checkWallsY(Rect* self);
void Rect_destroy(Rect* self);


typedef struct MRect {
    Rect super;
    Movable vectors;
} MRect;

typedef struct MRectPtrArr {
    uint size;
    uint idx;
    MRect** arr;
} MRectPtrArr;

void MRect_ctor(MRect* self, char* type, float x, float y,
                float width, float height, SDL_Texture* tex);
void MRect_update(MRect* self, float td, uint checkCollision, MRectPtrArr* fg);


typedef struct MRectArr {
    uint size;
    uint idx;
    MRect* arr;
} MRectArr;

void MRectArr_ctor(MRectArr* self);
uint MRectArr_add(MRectArr* self, MRect obj);
void MRectArr_del(MRectArr* self, uint id);


void MRectPtrArr_ctor(MRectPtrArr* self);
uint MRectPtrArr_add(MRectPtrArr* self, MRect* rectPtr);
void MRectPtrArr_sort(MRectPtrArr* self, char towards);
void MRectPtrArr_del(MRectPtrArr* self, MRect* rectPtr);
uint MRectPtrArr_checkCollision(MRectPtrArr* self, MRect* obj);
void MRectPtrArr_render(MRectPtrArr* self, SDL_Renderer* ren);
void MRectPtrArr_update(MRectPtrArr* self, float td);
void MRectPtrArr_destroy(MRectPtrArr* self);


typedef struct Player {
    MRect super;
    SDL_Texture* texUp;
    SDL_Texture* texNorm;
    SDL_Texture* texDown;
    float cooldown;
    MRectArr shots;
} Player;

void Player_ctor(Player* self, MRectPtrArr* fg, SDL_Renderer* ren);
void Player_move(Player* self, const uint8_t* keyStates);
void Player_shoot(Player* self, MRectPtrArr* fg, SDL_Renderer* ren);
void Player_update(Player* self, float td, MRectPtrArr* fg);
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
