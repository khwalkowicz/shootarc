//
// Created by Kamil Walkowicz on 02/01/2018.
//

#include <SDL2/SDL_image.h>
#include <unistd.h>
#include "window.h"
#include "config.h"

char* getAssetPath(char* str) {
    char* result = malloc(strlen(ASSETPATH) + strlen(str) + 1);
    strcpy(result, ASSETPATH);
    strcat(result, str);
    if(access(result, F_OK) == -1) {
        printf("getAssetPath Error: Asset %s does not exist.\n", result);
        return NULL;
    }
    return result;
}

void SDLError_log(FILE* stream, char* msg) {
    fprintf(stream, "%s Error: %s\n", msg, SDL_GetError());
}

void showFPSinTitle(SDL_Window* win, float timeDelta) {
    uint fps = (uint)round(10 / (double)timeDelta);
    char* str = malloc(strlen(WINDOW_TITLE) + 14);
    sprintf(str, "%s - FPS: %u", WINDOW_TITLE, fps);
    SDL_SetWindowTitle(win, str);
}

SDL_Texture* loadTexture(SDL_Renderer* ren, char* str) {
    char* path = getAssetPath(str);
    SDL_Texture* texture = IMG_LoadTexture(ren, path);
    if(texture == NULL) {
        SDLError_log(stdout, "IMG_LoadTexture");
        return NULL;
    }
    return texture;
}

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, uint x, uint y) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(ren, tex, NULL, &dest);
}

float approach(float goal, float curr, double dt) {
    float diff = goal - curr;
    if(diff > dt) {
        return curr + (float)dt;
    }
    if(diff < - dt) {
        return curr - (float)dt;
    }
    return goal;
}

void Win_controls(float* win_velocity_goal, Player* player,
                  SDL_Keycode key, uint keyDown) {
    if(key == SDLK_UP || key == SDLK_DOWN || key == SDLK_w || key == SDLK_s)
        Player_controls(player, key, keyDown);
    if(key == SDLK_RIGHT || key == SDLK_d) {
        if(keyDown)
            *win_velocity_goal = WIN_VELOCITY_GOAL;
        else
            *win_velocity_goal = WIN_VELOCITY;
    }
}