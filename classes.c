//
// Created by Kamil Walkowicz on 03/01/2018.
//

#include "classes.h"
#include "window.h"
#include "config.h"

void Shape_ctor(Shape* self, float x, float y) {
    self->x = x;
    self->y = y;
}


void Vector_ctor(Vector* self, float x, float y) {
    self->x = x;
    self->y = y;
}


void MShape_ctor(MShape* self, float x, float y) {
    Shape_ctor(&self->super, x, y);
    Vector_ctor(&self->velocity, 0.0, 0.0);
    Vector_ctor(&self->velocityGoal, 0.0, 0.0);
}

void MShape_update(MShape* self, float td) {
    self->velocity.x = approach(self->velocityGoal.x, self->velocity.x, td * 50);
    self->velocity.y = approach(self->velocityGoal.y, self->velocity.y, td * 50);
    self->super.x += self->velocity.x * td;
    self->super.y += self->velocity.y * td;
}


void Rect_ctor(Rect* self, float x, float y,
                float width, float height, SDL_Texture* tex) {
    Shape_ctor(&self->super, x, y);

    self->height = height;
    self->width  = width;
    self->tex    = tex;
}

void Rect_render(Rect* self, SDL_Renderer* ren) {
    SDL_Rect dest;
    dest.x = (int)self->super.x;
    dest.y = (int)self->super.y;
    dest.w = (int)self->width;
    dest.h = (int)self->height;
    SDL_RenderCopy(ren, self->tex, NULL, &dest);
}

void Rect_destroy(Rect* self) {
    SDL_DestroyTexture(self->tex);
}


void MRect_ctor(MRect* self, float x, float y,
                float width, float height, SDL_Texture* tex) {
    MShape_ctor(&self->super, x, y);

    self->height = height;
    self->width  = width;
    self->tex    = tex;
}

void MRect_render(MRect* self, SDL_Renderer* ren) {
    SDL_Rect dest;
    dest.x = (int)self->super.super.x;
    dest.y = (int)self->super.super.y;
    dest.w = (int)self->width;
    dest.h = (int)self->height;
    SDL_RenderCopy(ren, self->tex, NULL, &dest);
}

void MRect_destroy(MRect* self) {
    SDL_DestroyTexture(self->tex);
}


void Player_ctor(Player* self, SDL_Renderer* ren) {
    self->texUp   = loadTexture(ren, "player_up.png");
    self->texNorm = loadTexture(ren, "player.png");
    self->texDown = loadTexture(ren, "player_down.png");

    MRect_ctor(&self->super,
               25, (SCREEN_HEIGHT / 2) - (75 / 2),
               PLAYER_W, PLAYER_H,
               self->texNorm);
}

void Player_controls(Player* self, SDL_Keycode key, uint keyDown) {
    if(keyDown) {
        if(key == SDLK_UP || key == SDLK_w) {
            self->super.super.velocityGoal.y = -1 * PLAYER_VELOCITY_GOAL;
            self->super.tex = self->texUp;
        }
        if(key == SDLK_DOWN || key == SDLK_s) {
            self->super.super.velocityGoal.y = PLAYER_VELOCITY_GOAL;
            self->super.tex = self->texDown;
        }
    } else {
        if(key == SDLK_UP || key == SDLK_w ||
           key == SDLK_DOWN || key == SDLK_s) {
            self->super.super.velocityGoal.y = 0;
            self->super.tex = self->texNorm;
        }
    }
}

void Player_destroy(Player* self) {
    SDL_DestroyTexture(self->texUp);
    SDL_DestroyTexture(self->texDown);
    SDL_DestroyTexture(self->texNorm);
}


void Background_ctor(Background* self, float pos, SDL_Renderer* ren) {
    MRect_ctor(&self->super, pos * SCREEN_WIDTH, 0,
               SCREEN_WIDTH, SCREEN_HEIGHT, NULL);

    self->super.super.velocity.x = WIN_VELOCITY;
    self->super.super.velocityGoal.x = WIN_VELOCITY;

    self->tiles_x = (uint)ceil((double)self->super.width / BG_TILE_W);
    self->tiles_y = (uint)ceil((double)self->super.height / BG_TILE_H);
    self->size = self->tiles_x * self->tiles_y;
    self->array = malloc(self->size * sizeof(Rect));

    for(uint h_i = 0; h_i < self->tiles_y; h_i++)
        for(uint w_i = 0; w_i < self->tiles_x; w_i++) {

            int id = rand() % BG_TILES_Q + 1;
            char* path = malloc(strlen(BG_TILES_PATH) + 2 + 4);
            sprintf(path, "%s%d.png", BG_TILES_PATH, id);

            Rect tile;
            Rect_ctor(&tile,
                      self->super.super.super.x + w_i * BG_TILE_W,
                      self->super.super.super.y + h_i * BG_TILE_H,
                      BG_TILE_W, BG_TILE_H,
                      loadTexture(ren, path));

            self->array[h_i * self->tiles_x + w_i] = tile;
        }
}

void Background_render(Background* self, SDL_Renderer* ren) {
    for(uint h_i = 0; h_i < self->tiles_y; h_i++)
        for(uint w_i = 0; w_i < self->tiles_x; w_i++)
            Rect_render(&self->array[h_i * self->tiles_x + w_i], ren);
}

void Background_update(Background* self, float win_velocity_goal,
                       float td, SDL_Renderer* ren) {
    if(self->super.super.super.x - self->super.super.velocity.x * td +
       SCREEN_WIDTH <= 0)
        Background_ctor(self, 1, ren);

    self->super.super.velocityGoal.x = win_velocity_goal;
    MShape_update((MShape*)self, td);

    for(uint h_i = 0; h_i < self->tiles_y; h_i++)
        for(uint w_i = 0; w_i < self->tiles_x; w_i++)
            self->array[h_i * self->tiles_x + w_i].super.x =
                    self->super.super.super.x + w_i * BG_TILE_W;
}

void Background_destroy(Background* self) {
    for(uint h_i = 0; h_i < self->tiles_y; h_i++)
        for(uint w_i = 0; w_i < self->tiles_x; w_i++)
            Rect_destroy(&self->array[h_i * self->tiles_x + w_i]);
}