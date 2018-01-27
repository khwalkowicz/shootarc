//
// Created by Kamil Walkowicz on 25/01/2018.
//

#ifndef ARCSHOOT_ENGINE_H_INCLUDED
#define ARCSHOOT_ENGINE_H_INCLUDED

#include "classes.h"
#include "window.h"


typedef unsigned int STATE;
static STATE STATE_MAINMENU  = 1;
static STATE STATE_GAME      = 2;
static STATE STATE_PAUSEMENU = 3;


typedef struct MainMenu {
    Rect  logo;
    Rect  startBtn;
    Rect  cursor;
    float cursorState;
    Rect  copyright;
} MainMenu;

void MainMenu_init(MainMenu* self, SDL_Renderer* ren);
void MainMenu_main(MainMenu* self, Timer* timer, STATE* state,
                   SDL_Event event, SDL_Renderer* ren);
void MainMenu_clean(MainMenu* self);


typedef struct Game {
    MRectPtrArr fg;
    Player   player;
    uint     difficulty;
    LifeBox  lifeBox;
    EnemyArr enemies;
    uint     gameStopped;
} Game;

void Game_init(Game* self, SDL_Renderer* ren);
void Game_main(Game* self, Timer* timer, STATE* state,
               SDL_Event event, SDL_Renderer* ren);
void Game_clean(Game* self);


typedef struct PauseMenu {
    Rect menu;
} PauseMenu;

void PauseMenu_init(PauseMenu* self, SDL_Renderer* ren);
void PauseMenu_main(PauseMenu* self, SDL_Renderer* ren);
void PauseMenu_clean(PauseMenu* self);

#endif //ARCSHOOT_ENGINE_H
