//
// Created by Kamil Walkowicz on 23/01/2018.
//

#include "classes.h"
#include "window.h"
#include "config.h"


void Player_ctor(Player* self, uint lifes,
                 MRectPtrArr* fg, SDL_Renderer* ren) {
    self->texUp    = loadTexture(ren, "player_up.png");
    self->texNorm  = loadTexture(ren, "player.png");
    self->texDown  = loadTexture(ren, "player_down.png");

    self->super.super.explosionTex = loadTexture(ren, "player_explosion.png");

    self->cooldown = PLAYER_COOLDOWN;
    self->lifes    = lifes;

    MRect_ctor(&self->super,
               "player",
               25, (SCREEN_HEIGHT / 2) - (75 / 2),
               PLAYER_W, PLAYER_H,
               self->texNorm);

    MRectArr_ctor(&self->shots);
    MRectPtrArr_add(fg, (MRect*)self);
}

void Player_move(Player* self, const uint8_t* keyStates) {
    /* BASIC CONTROLS */
    if(keyStates[ SDL_SCANCODE_UP ]    || keyStates[ SDL_SCANCODE_W ]) {
        self->super.vectors.velocityGoal.y = -1 * PLAYER_VELOCITY_GOAL;
        self->super.super.tex = self->texUp;
    }
    if(keyStates[ SDL_SCANCODE_DOWN ]  || keyStates[ SDL_SCANCODE_S ]) {
        self->super.vectors.velocityGoal.y = PLAYER_VELOCITY_GOAL;
        self->super.super.tex = self->texDown;
    }
    if(keyStates[ SDL_SCANCODE_LEFT ]  || keyStates[ SDL_SCANCODE_A ]) {
        self->super.vectors.velocityGoal.x = -1 * PLAYER_VELOCITY_GOAL;
    }
    if(keyStates[ SDL_SCANCODE_RIGHT ] || keyStates[ SDL_SCANCODE_D ]) {
        self->super.vectors.velocityGoal.x = PLAYER_VELOCITY_GOAL;
    }

    /* IF NONE */
    if(!keyStates[ SDL_SCANCODE_UP ]   && !keyStates[ SDL_SCANCODE_W ] &&
       !keyStates[ SDL_SCANCODE_DOWN ] && !keyStates[ SDL_SCANCODE_S ]) {
        self->super.vectors.velocityGoal.y = 0;
        self->super.super.tex = self->texNorm;
    }
    if(!keyStates[ SDL_SCANCODE_LEFT ]  && !keyStates[ SDL_SCANCODE_A ] &&
       !keyStates[ SDL_SCANCODE_RIGHT ] && !keyStates[ SDL_SCANCODE_D ]) {
        self->super.vectors.velocityGoal.x = 0;
    }
}

void Player_shoot(Player* self, MRectPtrArr* fg, SDL_Renderer* ren) {
    if(self->cooldown <= 0) {
        self->cooldown = PLAYER_COOLDOWN;

        MRect shot;
        MRect_ctor(&shot,
                   "player_shot",
                   self->super.super.super.x + self->super.super.width,
                   self->super.super.super.y + (self->super.super.width / 2)
                   - 3,
                   41,
                   6,
                   loadTexture(ren, "player_shot.png")
        );
        shot.vectors.velocityGoal.x = PLAYER_SHOT_VELOCITY_GOAL;

        /*
         * Since adding obj to an array might cause realloc
         * I'm storing current address of the array and later
         * repoint pointers to the array's elements
         */
        MRect* temp = self->shots.arr;

        MRectArr_add(&self->shots, shot);

        if(self->shots.arr != temp) {
            MRect* max = temp + (self->shots.idx - 1) * sizeof(MRect);
            for (uint i = 0; i < fg->idx; i++)
                if(temp <= fg->arr[i] && fg->arr[i] <= max) {
                    size_t idx = (fg->arr[i] - temp);
                    fg->arr[i] = &self->shots.arr[idx];
                }
        }

        MRectPtrArr_add(fg, &self->shots.arr[self->shots.idx - 1]);
        MRectPtrArr_sort(fg, 'x');
    }
}

void Player_update(Player* self, float td, uint* gameStopped,
                   SDL_Renderer* ren, MRectPtrArr* fg) {
    if(!self->lifes && !self->super.super.explosionState)
        self->super.super.explosionState = 1;

    if(0 < self->super.super.explosionState &&
           self->super.super.explosionState < 16) {
        SDL_Rect clip = {
            ((int)self->super.super.explosionState % 4) *
                    PLAYER_EXPLOSION_TILE_W,
            ((int)self->super.super.explosionState / 4) *
                    PLAYER_EXPLOSION_TILE_H,
            PLAYER_EXPLOSION_TILE_W, PLAYER_EXPLOSION_TILE_H
        };
        renderTexture(self->super.super.explosionTex, ren,
                      (int)self->super.super.super.x -
                      (PLAYER_EXPLOSION_TILE_W - PLAYER_W) / 2,
                      (int)self->super.super.super.y -
                      (PLAYER_EXPLOSION_TILE_H - PLAYER_H) / 2,
                      PLAYER_EXPLOSION_TILE_W, PLAYER_EXPLOSION_TILE_H,
                      &clip
        );

        if((int)self->super.super.explosionState == 6)
            *gameStopped = 1;

        if((int)self->super.super.explosionState == 8) {
            Rect_destroy((Rect*)self);
            MRectPtrArr_del(fg, (MRect*)self);
        }

        self->super.super.explosionState += td / 2;
    }

    if(self->cooldown > 0)
        self->cooldown -= td;
}

void Player_handleShot(Player* self, MRectPtrArr* fg,
                       MRect* obj, MRect* coll) {
    if(coll && !strcmp(coll->super.type, "enemy"))
        coll->super.explosionState = 1;

    Rect_destroy((Rect*)obj);
    size_t id = obj - self->shots.arr;

    MRect* max = self->shots.arr + (self->shots.idx - 1) * sizeof(MRect);
    for(uint i = 0; i < fg->idx; i++) {
        if(self->shots.arr <= fg->arr[i] && fg->arr[i] <= max) {
            size_t idx = fg->arr[i] - self->shots.arr;
            if(idx > id)
                fg->arr[i] = &self->shots.arr[idx - 1];
        }
    }
    MRectPtrArr_del(fg, obj);

    MRectArr_del(&self->shots, (uint)id);
}

void Player_hit(Player* self) {
    self->lifes--;
}

void Player_destroy(Player* self) {
    SDL_DestroyTexture(self->texUp);
    SDL_DestroyTexture(self->texDown);
    SDL_DestroyTexture(self->texNorm);
    SDL_DestroyTexture(self->super.super.explosionTex);
    free(self->shots.arr);
}


void LifeBox_ctor(LifeBox* self, Player* player, SDL_Renderer* ren) {
    Rect_ctor(&self->super, "ui-box",
        15, 15, PLAYER_LIFEBOX_W, PLAYER_LIFEBOX_H,
        loadTexture(ren, "ui/box-life.png")
    );
    MRectArr_ctor(&self->lifes);
    for(uint i = 0; i < player->lifes; i++) {
        MRect obj;
        MRect_ctor(&obj, "ui-life",
            15 + 5 + 3 + i * PLAYER_LIFE_ICO_W + i * 5, 15 + 5 + 5,
            PLAYER_LIFE_ICO_W, PLAYER_LIFE_ICO_H,
            loadTexture(ren, "ui/life.png")
        );
        MRectArr_add(&self->lifes, obj);
    }
}

void LifeBox_update(LifeBox* self, Player* player) {
    for(int i = (int)self->lifes.idx - 1; i >= (int)player->lifes; i--)
        SDL_SetTextureAlphaMod(self->lifes.arr[i].super.tex, 85);
}

void LifeBox_render(LifeBox* self, SDL_Renderer* ren) {
    Rect_render(&self->super, ren);
    for(uint i = 0; i < self->lifes.idx; i++)
        Rect_render((Rect*)&self->lifes.arr[i], ren);
}

void LifeBox_destroy(LifeBox* self) {
    for(uint i = 0; i < self->lifes.idx; i++)
        SDL_DestroyTexture(self->lifes.arr[i].super.tex);
    free(self->lifes.arr);
    SDL_DestroyTexture(self->super.tex);
}