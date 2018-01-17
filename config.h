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

static uint PLAYER_W = 75;
static uint PLAYER_H = 75;
static float PLAYER_VELOCITY_GOAL = 50;
static float PLAYER_COOLDOWN = 3.3333;
static float PLAYER_SHOT_VELOCITY_GOAL = 100;

static uint ENEMY_W = 59;
static uint ENEMY_H = 64;
static uint ENEMY_EXPLOSION_W = 88;
static uint ENEMY_EXPLOSION_H = 88;

static float WIN_VELOCITY = -5;
static float WIN_VELOCITY_GOAL = -50;

#endif //ARCSHOOT_CONFIG_H
