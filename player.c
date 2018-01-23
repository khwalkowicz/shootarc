//
// Created by Kamil Walkowicz on 23/01/2018.
//

#include "classes.h"
#include "window.h"
#include "config.h"


void Player_ctor(Player* self, MRectPtrArr* fg, SDL_Renderer* ren) {
    self->texUp   = loadTexture(ren, "player_up.png");
    self->texNorm = loadTexture(ren, "player.png");
    self->texDown = loadTexture(ren, "player_down.png");

    self->cooldown = PLAYER_COOLDOWN;

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

void Player_update(Player* self, float td, MRectPtrArr* fg) {
    MRect_update((MRect*)self, td, 1, self, NULL, fg, NULL);
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

void Player_destroy(Player* self) {
    SDL_DestroyTexture(self->texUp);
    SDL_DestroyTexture(self->texDown);
    SDL_DestroyTexture(self->texNorm);
}