//
// Created by Kamil Walkowicz on 03/01/2018.
//

#ifndef ARCSHOOT_WINDOW_H_INCLUDED
#define ARCSHOOT_WINDOW_H_INCLUDED

#include <stdio.h>
#include <SDL2/SDL.h>
#include "classes.h"


char* getAssetPath(char* str);

void SDLError_log(FILE* stream, char* msg);

void showFPSinTitle(SDL_Window* win, float timeDelta);

SDL_Texture* loadTexture(SDL_Renderer* ren, char* str);

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, uint x, uint y);

float approach(float goal, float curr, double dt);

void Win_controls(float* win_velocity_goal, Player* player,
                  SDL_Keycode key, uint keyDown);

#endif //ARCSHOOT_WINDOW_H
