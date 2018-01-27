//
// Created by Kamil Walkowicz on 03/01/2018.
//

#ifndef ARCSHOOT_CLASSES_H_INCLUDED
#define ARCSHOOT_CLASSES_H_INCLUDED

#include <SDL2/SDL.h>


typedef struct Timer {
    uint  ticksPrev;
    uint  ticksCurr;
    uint  isPaused;
    float dt;
} Timer;

void Timer_ctor(Timer* timer);
void Timer_update(Timer* timer);
void Timer_toggle(Timer* timer);


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
    float explosionState;
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


typedef struct Enemy {
    MRect  super;
    float   enterTime;
    Vector direction;
    uint   size;
    uint   idx;
    Shape* path;
} Enemy;

typedef struct EnemyArr {
    uint size;
    uint idx;
    Enemy* arr;
} EnemyArr;

void Enemy_ctor(EnemyArr* enemies, float enterTime, float x, float y,
                MRectPtrArr* fg, SDL_Renderer* ren);
uint Enemy_addPoint(Enemy* self, float x, float y);
void Enemy_delPoint(Enemy* self, EnemyArr* enemies, MRectPtrArr* fg);
uint Enemy_update(Enemy* self, float td, EnemyArr* enemies, MRectPtrArr* fg);
void Enemy_explode(Enemy* self, EnemyArr* enemies,
                   MRectPtrArr* fg, SDL_Renderer* ren);

void EnemyArr_ctor(EnemyArr* self);
uint EnemyArr_add(EnemyArr* self, Enemy obj, MRectPtrArr* fg);
void EnemyArr_update(EnemyArr* self, float td, MRectPtrArr* fg);
void EnemyArr_del(EnemyArr* self, uint id, MRectPtrArr* fg);


typedef struct Player {
    MRect super;
    SDL_Texture* texUp;
    SDL_Texture* texNorm;
    SDL_Texture* texDown;
    uint lifes;
    float cooldown;
    MRectArr shots;
} Player;

int MRect_update(MRect* self, float td, uint checkCollision, Player* player,
                 EnemyArr* enemies, MRectPtrArr* fg, SDL_Renderer* ren);

void MRectPtrArr_update(MRectPtrArr* self, float td, Player* player,
                        EnemyArr* enemies, SDL_Renderer* ren);

void Player_ctor(Player* self, uint lifes,
                 MRectPtrArr* fg, SDL_Renderer* ren);
void Player_move(Player* self, const uint8_t* keyStates);
void Player_shoot(Player* self, MRectPtrArr* fg, SDL_Renderer* ren);
void Player_update(Player* self, float td, uint* gameStopped,
                   SDL_Renderer* ren, MRectPtrArr* fg);
void Player_handleShot(Player* self, MRectPtrArr* fg,
                       MRect* obj, MRect* coll);
void Player_hit(Player* self);
void Player_destroy(Player* self);


typedef struct LifeBox {
    Rect super;
    MRectArr lifes;
} LifeBox;

void LifeBox_ctor(LifeBox* self, Player* player, SDL_Renderer* ren);
void LifeBox_update(LifeBox* self, Player* player);
void LifeBox_render(LifeBox* self, SDL_Renderer* ren);
void LifeBox_destroy(LifeBox* self);


typedef struct Background {
    MRect super;
    uint  tiles_x;
    uint  tiles_y;
    uint  size;
    Rect* array;
} Background;

void Background_ctor(Background* self, float pos, SDL_Renderer* ren);
void Background_render(Background* self, SDL_Renderer* ren);
void Background_update(Background* self, float td, SDL_Renderer* ren);
void Background_destroy(Background* self);


#endif //ARCSHOOT_CLASSES_H
