//
// Created by Kamil Walkowicz on 29/01/2018.
//

#ifndef ARCSHOOT_LEVELS_H_INCLUDED
#define ARCSHOOT_LEVELS_H_INCLUDED

#include <SDL2/SDL_render.h>
#include "config.h"
#include "classes.h"

typedef struct Level {
    uint     number;
    EnemyArr enemies;
} Level;

void Level_ctor(Level* self, uint* levels);
void Level_update(Level* self, float dt, MRectPtrArr* fg);
uint Level_isFinished(Level* self);
void Level_destroy(Level* self);


typedef struct LevelScreen {
    Rect         bg;
    Rect         text;
    SDL_Texture* numbers[10];
    MRectArr     numbersCurr;
    float        opacity;
} LevelScreen;

void LevelScreen_init(LevelScreen* self, SDL_Renderer* ren);
void LevelScreen_main(LevelScreen* self, uint number,
                      double dt, SDL_Renderer* ren);
void LevelScreen_clear(LevelScreen* self);


typedef struct LevelFile {
    FILE* levelFile;
} LevelFile;

uint  LevelFile_open(LevelFile* self, uint number);
Level LevelFile_read(LevelFile* self, uint* levels,
                     MRectPtrArr* fg, SDL_Renderer* ren);
void  LevelFile_destroy(LevelFile* self);

#endif //ARCSHOOT_LEVELS_H
