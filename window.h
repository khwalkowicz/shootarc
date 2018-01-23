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

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren,
                   int x, int y, int w, int h, SDL_Rect *clip);

float approach(float goal, float curr, double dt);

int sign(float x);

void Win_controls(Player* player, MRectPtrArr* fg,
                  SDL_Renderer* ren, const uint8_t* keyStates);

#endif //ARCSHOOT_WINDOW_H
