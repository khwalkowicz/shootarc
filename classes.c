//
// Created by Kamil Walkowicz on 02/01/2018.
//

#include "classes.h"
#include "config.h"
#include "functions.h"

Rect Rect_initPlayer(SDL_Renderer* ren) {
    Rect player;
    player.w = 50;
    player.h = 75;
    player.super.x = 25;
    player.super.y = SCREEN_HEIGHT / 2 - player.h / 2;
    player.tex = loadTexture(ren, "player.png");
    return player;
}

void Rect_render(Rect obj, SDL_Renderer* ren) {
    SDL_Rect dest;
    dest.x = obj.super.x;
    dest.y = obj.super.y;
    dest.w = obj.w;
    dest.h = obj.h;
    SDL_RenderCopy(ren, obj.tex, NULL, &dest);
}

void Rect_destroy(Rect obj) {
    SDL_DestroyTexture(obj.tex);
}