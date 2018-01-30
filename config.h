//
// Created by Kamil Walkowicz on 03/01/2018.
//

#ifndef ARCSHOOT_CONFIG_H
#define ARCSHOOT_CONFIG_H

typedef unsigned int uint;

static char WINDOW_TITLE[] = "ArcShoot alpha";
static uint SCREEN_WIDTH  = 960;
static uint SCREEN_HEIGHT = 540;

static char ASSETPATH[] = "../assets/";

static char* BG_TILES_PATH = "sky/";
static uint  BG_TILES_Q    = 24;
static uint  BG_TILE_W     = 120;
static uint  BG_TILE_H     = 180;

static uint  LOGO_W           = 334;
static uint  LOGO_H           = 90;
static uint  MENU_BTN_W       = 222;
static uint  MENU_BTN_H       = 40;
static uint  MENU_CURSOR_W    = 40;
static uint  MENU_CURSOR_H    = 35;
static uint  MENU_COPYRIGHT_W = 286;
static uint  MENU_COPYRIGHT_H = 9;

static uint  PAUSE_MENU_W = 581;
static uint  PAUSE_MENU_H = 457;

static uint  LEVEL_TEXT_W = 106;
static uint  LEVEL_TEXT_H = 23;

static uint  GAMEOVER_TEXT_W = 207;
static uint  GAMEOVER_TEXT_H = 24;

static uint  PLAYER_W                  = 75;
static uint  PLAYER_H                  = 75;
static float PLAYER_VELOCITY_GOAL      = 50;
static float PLAYER_COOLDOWN           = 3.3333;
static float PLAYER_SHOT_VELOCITY_GOAL = 100;
static uint  PLAYER_LIFEBOX_W          = 221;
static uint  PLAYER_LIFEBOX_H          = 46;
static uint  PLAYER_LIFE_ICO_W         = 37;
static uint  PLAYER_LIFE_ICO_H         = 26;
static uint  PLAYER_EXPLOSION_TILE_W   = 105;
static uint  PLAYER_EXPLOSION_TILE_H   = 103;
static uint  PLAYER_LIFES_EASY         = 5;
static uint  PLAYER_LIFES_NORMAL       = 3;
static uint  PLAYER_LIFES_HARD         = 1;

static uint  ENEMY_W                = 44;
static uint  ENEMY_H                = 48;
static uint  ENEMY_EXPLOSION_W      = 66;
static uint  ENEMY_EXPLOSION_H      = 66;
static uint  ENEMY_EXPLOSION_TILE_W = 88;
static uint  ENEMY_EXPLOSION_TILE_H = 88;
static float ENEMY_VELOCITY_GOAL    = 20;

static float WIN_VELOCITY = -5;

static uint  NUMBERS_H = 23;
static uint  NUMBERS_W[10];

#endif //ARCSHOOT_CONFIG_H
