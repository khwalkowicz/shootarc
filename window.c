//
// Created by Kamil Walkowicz on 02/01/2018.
//

#include <SDL2/SDL_image.h>
#include <unistd.h>
#include "window.h"
#include "config.h"


void Timer_ctor(Timer* timer) {
    timer->ticksPrev = 0;
    timer->ticksCurr = 0;
    timer->isPaused  = 0;
}

void Timer_update(Timer* timer) {
    timer->ticksCurr = SDL_GetTicks();

    if(!timer->isPaused)
        timer->dt = (float)(timer->ticksCurr - timer->ticksPrev) / 100;
    else
        timer->dt = 0;

    timer->ticksPrev = timer->ticksCurr;
}

void Timer_toggle(Timer* timer) {
    if(timer->isPaused)
        timer->isPaused = 0;
    else
        timer->isPaused = 1;
}


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

void showFPSinTitle(SDL_Window* win, float dt) {
    uint fps = (uint)round(10 / (double)dt);
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

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren,
                   int x, int y, int w, int h, SDL_Rect *clip) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, clip, &dst);
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

void Win_controls(Player* player, MRectPtrArr* fg, Timer* timer,
                  SDL_Renderer* ren, const uint8_t* keyStates) {
    if(!timer->isPaused) {
        Player_move(player, keyStates);

        if(keyStates[SDL_SCANCODE_SPACE])
            Player_shoot(player, fg, ren);
    }
}