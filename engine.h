//
// Created by Kamil Walkowicz on 25/01/2018.
//

#ifndef ARCSHOOT_ENGINE_H_INCLUDED
#define ARCSHOOT_ENGINE_H_INCLUDED

#include "classes.h"
#include "window.h"
#include "levels.h"


typedef unsigned int STATE;
static STATE STATE_MAINMENU   = 1;
static STATE STATE_GAME       = 2;
static STATE STATE_PAUSEMENU  = 3;
static STATE STATE_GAMEOVER   = 4;
static STATE STATE_YOUHAVEWON = 5;

static STATE STATE_MAINMENU_STARTMENU = 1;
static STATE STATE_MAINMENU_DIFFMENU  = 2;


typedef struct MainMenu_StartMenu {
    Button startBtn;
    float freeze;
    uint initialized;
} MainMenu_StartMenu;

void MainMenu_StartMenu_init(MainMenu_StartMenu* self, SDL_Renderer* ren);
// void MainMenu_StartMenu_main() later in this file
void MainMenu_StartMenu_clean(MainMenu_StartMenu* self);


typedef struct MainMenu_DiffMenu {
    Button btns[3];
    uint   options[3];
    uint   choice;
    float  freeze;
    uint   initialized;
} MainMenu_DiffMenu;

void MainMenu_DiffMenu_init(MainMenu_DiffMenu* self, SDL_Renderer* ren);
// void MainMenu_DiffMenu_main() later in this file
void MainMenu_DiffMenu_clean(MainMenu_DiffMenu* self);


typedef struct MainMenu {
    Rect  logo;
    Rect  cursor;
    float cursorState;
    Rect  copyright;
    STATE submenu;
    MainMenu_StartMenu startMenu;
    MainMenu_DiffMenu diffMenu;
    uint  initialized;
} MainMenu;

void MainMenu_init(MainMenu* self, SDL_Renderer* ren);
void MainMenu_main(MainMenu* self, Timer* timer, STATE* state,
                   uint* difficulty, SDL_Event event, SDL_Renderer* ren);
void MainMenu_clean(MainMenu* self);

void MainMenu_StartMenu_main(MainMenu_StartMenu* self, SDL_Event event,
                             Timer* timer, MainMenu* super, SDL_Renderer* ren);
void MainMenu_DiffMenu_main(MainMenu_DiffMenu* self, SDL_Event event,
                            Timer* timer, MainMenu* super, uint* difficulty,
                            STATE* state, SDL_Renderer* ren);


typedef struct Game {
    MRectPtrArr fg;
    LevelScreen levelScreen;
    Player   player;
    uint     difficulty;
    LifeBox  lifeBox;
    uint     levels;
    Level    level;
    uint     newLevelStarted;
    uint     freeze;
    uint     gameStopped;
    uint     initialized;
} Game;

void Game_init(Game* self, LevelFile* levelFile, SDL_Renderer* ren);
void Game_main(Game* self, Timer* timer, STATE* state,
               LevelFile* levelFile, SDL_Event event, SDL_Renderer* ren);
void Game_clean(Game* self);


typedef struct PauseMenu {
    Rect menu;
} PauseMenu;

void PauseMenu_init(PauseMenu* self, SDL_Renderer* ren);
void PauseMenu_main(PauseMenu* self, SDL_Renderer* ren);
void PauseMenu_clean(PauseMenu* self);


typedef struct GameOverScreen {
    Rect  bg;
    Rect  text;
    float opacity;
    float freeze;
    uint  initialized;
} GameOverScreen;

void GameOverScreen_init(GameOverScreen* self, SDL_Renderer* ren);
void GameOverScreen_main(GameOverScreen* self, Timer* timer,
                         STATE* state, SDL_Renderer* ren);
void GameOverScreen_clean(GameOverScreen* self);


typedef struct YouHaveWonScreen {
    Rect  bg;
    Rect  text;
    float opacity;
    float freeze;
    uint  initialized;
} YouHaveWonScreen;

void YouHaveWonScreen_init(YouHaveWonScreen* self, SDL_Renderer* ren);
void YouHaveWonScreen_main(YouHaveWonScreen* self, Timer* timer,
                         STATE* state, SDL_Renderer* ren);
void YouHaveWonScreen_clean(YouHaveWonScreen* self);


#endif //ARCSHOOT_ENGINE_H
