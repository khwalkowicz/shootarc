//
// Created by Kamil Walkowicz on 02/01/2018.
//

#ifndef GAME_FUNCTIONS_H_INCLUDED
#define GAME_FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <SDL2/SDL.h>


char* getAssetPath(char* str);

void SDLError_log(FILE* stream, char* msg);

SDL_Texture* loadTexture(SDL_Renderer* ren, char* str);

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, uint x, uint y);

#endif //GAME_FUNCTIONS_H
