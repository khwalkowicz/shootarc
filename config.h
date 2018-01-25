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
static uint  BG_TILES_Q = 24;
static uint  BG_TILE_W = 120;
static uint  BG_TILE_H = 180;

static uint  PAUSE_MENU_W = 640;
static uint  PAUSE_MENU_H = 360;

static uint  PLAYER_W = 75;
static uint  PLAYER_H = 75;
static float PLAYER_VELOCITY_GOAL = 50;
static float PLAYER_COOLDOWN = 3.3333;
static float PLAYER_SHOT_VELOCITY_GOAL = 100;

static uint  ENEMY_W = 44;
static uint  ENEMY_H = 48;
static uint  ENEMY_EXPLOSION_W = 66;
static uint  ENEMY_EXPLOSION_H = 66;
static uint  ENEMY_EXPLOSION_TILE_W = 88;
static uint  ENEMY_EXPLOSION_TILE_H = 88;
static float ENEMY_VELOCITY_GOAL = 20;

static float WIN_VELOCITY = -5;

#endif //ARCSHOOT_CONFIG_H
