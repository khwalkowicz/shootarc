//
// Created by Kamil Walkowicz on 02/01/2018.
//

#include <SDL2/SDL_image.h>
#include <unistd.h>
#include "functions.h"
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

void Win_renderBackground(SDL_Renderer* ren) {
    char* bg_path = "sky/";
    for(uint h_i = 0; h_i < ceil(SCREEN_HEIGHT / 180); h_i++)
        for(uint w_i = 0; w_i < ceil(SCREEN_WIDTH / 120); w_i++) {
            uint y = h_i * 180;
            uint x = w_i * 120;
            int id = rand() % 24 + 1;
            char* path = malloc(strlen(bg_path) + 2 + 4);
            sprintf(path, "%s%d.png", bg_path, id);
            SDL_Texture* img = loadTexture(ren, path);
            renderTexture(img, ren, x, y);
        }
}