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

Background Background_init(SDL_Renderer* ren) {
    Background obj;
    obj.move = 0.0;
    obj.path = "sky/";
    obj.tile_h = 180;
    obj.tile_w = 120;
    obj.assets_available = 24;
    obj.tiles_y = ceil(SCREEN_HEIGHT / obj.tile_h);
    obj.tiles_x = ceil(SCREEN_WIDTH / obj.tile_w);
    obj.size = obj.tiles_x * obj.tiles_y;
    obj.array = malloc(obj.size * sizeof(Rect));
    for(uint h_i = 0; h_i < obj.tiles_y; h_i++)
        for(uint w_i = 0; w_i < obj.tiles_x; w_i++) {
            Rect tile;
            tile.h = obj.tile_h;
            tile.w = obj.tile_w;
            tile.super.x = w_i * tile.w;
            tile.super.y = h_i * tile.h;
            int id = rand() % obj.assets_available + 1;
            char* path = malloc(strlen(obj.path) + 2 + 4);
            sprintf(path, "%s%d.png", obj.path, id);
            tile.tex = loadTexture(ren, path);
            obj.array[h_i * obj.tiles_x + w_i] = tile;
        }
    return obj;
}

void Background_scroll(Background* obj, SDL_Renderer* ren, float timeDelta) {
    obj->move += timeDelta;
    if(obj->move >= 1.0) {
        obj->move = 0.0;
        for (uint h_i = 0; h_i < obj->tiles_y; h_i++) {
            SDL_DestroyTexture(obj->array[h_i * obj->tiles_x].tex);
            for (uint w_i = 0; w_i < obj->tiles_x - 1; w_i++) {
                obj->array[h_i * obj->tiles_x + w_i] = obj->array[h_i * obj->tiles_x + w_i + 1];
                obj->array[h_i * obj->tiles_x + w_i].super.x = w_i * obj->array[h_i * obj->tiles_x + w_i].w;
                obj->array[h_i * obj->tiles_x + w_i].super.y = h_i * obj->array[h_i * obj->tiles_x + w_i].h;
            }
            Rect tile;
            tile.h = obj->tile_h;
            tile.w = obj->tile_w;
            tile.super.x = (obj->tiles_x - 1) * tile.w;
            tile.super.y = h_i * tile.h;
            int id = rand() % obj->assets_available + 1;
            char *path = malloc(strlen(obj->path) + 2 + 4);
            sprintf(path, "%s%d.png", obj->path, id);
            tile.tex = loadTexture(ren, path);
            obj->array[h_i * obj->tiles_x + (obj->tiles_x - 1)] = tile;
        }
    }
}

void Background_render(Background obj, SDL_Renderer* ren) {
    for(uint i = 0; i < obj.size; i++) {
        Rect_render(obj.array[i], ren);
    }
}

void Background_destroy(Background obj) {
    for(uint i = 0; i < obj.size; i++) {
        SDL_DestroyTexture(obj.array[i].tex);
    }
}