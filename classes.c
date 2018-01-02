//
// Created by Kamil Walkowicz on 02/01/2018.
//

#include "classes.h"
#include "config.h"
#include "functions.h"

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

Player Player_init(SDL_Renderer* ren) {
    Player player;
    player.super.w = 75;
    player.super.h = 75;
    player.super.super.x = 25;
    player.super.super.y = SCREEN_HEIGHT / 2 - player.super.h / 2;
    player.acc = 200.0;
    player.tex_up = loadTexture(ren, "player_up.png");
    player.tex_norm = loadTexture(ren, "player.png");
    player.tex_down = loadTexture(ren, "player_down.png");
    player.super.tex = player.tex_norm;
    return player;
}

void Player_move(Player *player, Background* bg, SDL_Keycode key,
                 uint keyDown, double timeDelta) {
    if(keyDown) {
        if(key == SDLK_UP || key == SDLK_w) {
            player->super.super.y -= player->acc * timeDelta * timeDelta;
            player->super.tex = player->tex_up;
        }
        if(key == SDLK_DOWN || key == SDLK_s) {
            player->super.super.y += player->acc * timeDelta * timeDelta;
            player->super.tex = player->tex_down;
        }
        if(key == SDLK_RIGHT || key == SDLK_d) {
            bg->speed *= 2;
        }
    } else {
        if(key == SDLK_UP || key == SDLK_w ||
           key == SDLK_DOWN || key == SDLK_s)
            player->super.tex = player->tex_norm;
        if(key == SDLK_RIGHT || key == SDLK_d)
            bg->speed = 0.10;
    }
}

void Player_destroy(Player obj) {
    SDL_DestroyTexture(obj.tex_up);
    SDL_DestroyTexture(obj.tex_down);
    SDL_DestroyTexture(obj.tex_norm);
}

Background Background_init(SDL_Renderer* ren) {
    Background obj;
    obj.move = 0.0;
    obj.speed = 0.1;
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
    if(obj->move >= (1 / obj->speed)) {
        obj->move = 0.0;
        for (uint h_i = 0; h_i < obj->tiles_y; h_i++) {
            SDL_DestroyTexture(obj->array[h_i * obj->tiles_x].tex);
            for (uint w_i = 0; w_i < obj->tiles_x - 1; w_i++) {
                obj->array[h_i * obj->tiles_x + w_i] =
                        obj->array[h_i * obj->tiles_x + w_i + 1];
                obj->array[h_i * obj->tiles_x + w_i].super.x =
                        w_i * obj->array[h_i * obj->tiles_x + w_i].w;
                obj->array[h_i * obj->tiles_x + w_i].super.y =
                        h_i * obj->array[h_i * obj->tiles_x + w_i].h;
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