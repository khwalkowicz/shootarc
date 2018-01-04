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
static float WIN_VELOCITY = -5.0;
static float WIN_VELOCITY_GOAL = -50.0;

#endif //ARCSHOOT_CONFIG_H
