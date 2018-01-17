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


typedef struct Rect {
    Shape super;
    char* type;
    float width;
    float height;
    uint explosionState;
    SDL_Texture* tex;
    SDL_Texture* explosionTex;
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

void MRect_ctor(MRect* self, char* type, float x, float y,
                float width, float height, SDL_Texture* tex);
// int MRect_update() later in this file


typedef struct MRectArr {
    uint size;
    uint idx;
    MRect* arr;
} MRectArr;

void MRectArr_ctor(MRectArr* self);
uint MRectArr_add(MRectArr* self, MRect obj);
void MRectArr_del(MRectArr* self, uint id);


typedef struct MRectPtrArr {
    uint size;
    uint idx;
    MRect** arr;
} MRectPtrArr;

void MRectPtrArr_ctor(MRectPtrArr* self);
uint MRectPtrArr_add(MRectPtrArr* self, MRect* rectPtr);
void MRectPtrArr_sort(MRectPtrArr* self, char towards);
void MRectPtrArr_del(MRectPtrArr* self, MRect* rectPtr);
MRect* MRectPtrArr_checkCollision(MRectPtrArr* self, MRect* obj);
void MRectPtrArr_render(MRectPtrArr* self, SDL_Renderer* ren);
// void MRectPtrArr_update() later in this file
void MRectPtrArr_destroy(MRectPtrArr* self);


typedef struct Player {
    MRect super;
    SDL_Texture* texUp;
    SDL_Texture* texNorm;
    SDL_Texture* texDown;
    float cooldown;
    MRectArr shots;
} Player;

int MRect_update(MRect* self, float td, uint checkCollision, Player* player,
                 MRectArr* enemies, MRectPtrArr* fg, SDL_Renderer* ren);

void MRectPtrArr_update(MRectPtrArr* self, float td, Player* player,
                        MRectArr* enemies, SDL_Renderer* ren);

void Player_ctor(Player* self, MRectPtrArr* fg, SDL_Renderer* ren);
void Player_move(Player* self, const uint8_t* keyStates);
void Player_shoot(Player* self, MRectPtrArr* fg, SDL_Renderer* ren);
void Player_update(Player* self, float td, MRectPtrArr* fg);
void Player_handleShot(Player* self, MRectPtrArr* fg,
                       MRect* obj, MRect* coll);
void Player_destroy(Player* self);


void Obstacle_ctor(MRectArr* enemies, char* type, float x, float y,
                   MRectPtrArr* fg, SDL_Renderer* ren);
void Obstacle_explode(MRect* self, MRectArr* enemies,
                      MRectPtrArr* fg, SDL_Renderer* ren);

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
