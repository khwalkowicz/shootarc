//
// Created by Kamil Walkowicz on 25/01/2018.
//

#ifndef ARCSHOOT_ENGINE_H_INCLUDED
#define ARCSHOOT_ENGINE_H_INCLUDED

#include "classes.h"
#include "window.h"

typedef struct Game {
    MRectPtrArr fg;
    Player player;
    EnemyArr enemies;
} Game;

void Game_init(Game* self, SDL_Renderer* ren);
void Game_main(Game* self, Timer* timer, SDL_Renderer* ren);
void Game_clean(Game* self);

#endif //ARCSHOOT_ENGINE_H
