//
// Created by Kamil Walkowicz on 18/01/2018.
//

#include "classes.h"
#include "config.h"
#include "window.h"

void Enemy_ctor(EnemyArr* enemies, float enterTime, float x, float y,
                MRectPtrArr* fg, SDL_Renderer* ren) {
    Enemy obj;

    MRect_ctor(&obj.super, "enemy",
               x, y, ENEMY_W, ENEMY_H,
               loadTexture(ren, "enemies/black1.png")
    );
    obj.super.super.explosionTex = loadTexture(ren, "enemies/explode.png");


    obj.enterTime = enterTime;
    obj.size = 1;
    obj.idx  = 0;
    obj.path = calloc(obj.size, sizeof(Shape));

    Enemy_addPoint(&obj, x, y);

    EnemyArr_add(enemies, obj, fg);

    MRectPtrArr_add(fg, (MRect*)&enemies->arr[enemies->idx - 1]);
    MRectPtrArr_sort(fg, 'x');
}

uint Enemy_addPoint(Enemy* self, float x, float y) {
    Shape point;
    Shape_ctor(&point, x, y);

    if(self->idx + 1 >= self->size) {
        self->size *= 2;

        Shape* temp = realloc(self->path, self->size * sizeof(Shape));
        if(temp != NULL)
            self->path = temp;
        else {
            free(self->path);
            printf("\nMemory alloc problem! Aborting!\n");
            return 1;
        }
    }

    self->path[self->idx] = point;
    self->idx++;

    return 0;
}

void Enemy_delPoint(Enemy* self, EnemyArr* enemies, MRectPtrArr* fg) {
    if(self->idx) {
        for(uint id = 0; id < self->idx - 1; id++)
            self->path[id] = self->path[id + 1];
        self->idx--;
    } else {
        Rect_destroy((Rect*)self);
        MRectPtrArr_del(fg, (MRect*)self);
        size_t id = self - enemies->arr;
        EnemyArr_del(enemies, (uint)id, fg);
    }
}

uint Enemy_update(Enemy* self, float td, EnemyArr* enemies, MRectPtrArr* fg) {
    if(self->enterTime - td >= 0) {
        self->enterTime -= td;
        return 0;
    }

    int diff = (int)(ENEMY_VELOCITY_GOAL * td / sqrt(2));

    if(abs((int)(self->super.super.super.x - self->path->x)) < diff &&
       abs((int)(self->super.super.super.y - self->path->y)) < diff)
        Enemy_delPoint(self, enemies, fg);

    Vector_ctor(&self->direction,
                self->path->x - self->super.super.super.x,
                self->path->y - self->super.super.super.y
    );

    self->super.vectors.velocityGoal.x = ENEMY_VELOCITY_GOAL *
                                         self->direction.x / (float)sqrt(
            self->direction.x *
            self->direction.x +
            (double)self->direction.y
            * self->direction.y
    );
    self->super.vectors.velocityGoal.y = self->super.vectors.velocityGoal.x *
                                         self->direction.y /
                                         self->direction.x;
    return 0;
}

void Enemy_explode(Enemy* self, EnemyArr* enemies,
                   MRectPtrArr* fg, SDL_Renderer* ren) {
    SDL_Rect clip = {
            (self->super.super.explosionState % 5) * ENEMY_EXPLOSION_TILE_W,
            (self->super.super.explosionState / 5) * ENEMY_EXPLOSION_TILE_H,
            ENEMY_EXPLOSION_TILE_W, ENEMY_EXPLOSION_TILE_H
    };
    renderTexture(self->super.super.explosionTex, ren,
                  (int)self->super.super.super.x -
                          (ENEMY_EXPLOSION_W - ENEMY_W) / 2,
                  (int)self->super.super.super.y -
                          (ENEMY_EXPLOSION_H - ENEMY_H) / 2,
                  ENEMY_EXPLOSION_W,
                  ENEMY_EXPLOSION_H,
                  &clip
    );
    if(self->super.super.explosionState == 5) {
        Rect_destroy((Rect*)self);
        MRectPtrArr_del(fg, (MRect*)self);
    }
    if(self->super.super.explosionState == 10) {
        size_t idx = self - enemies->arr;
        EnemyArr_del(enemies, (uint)idx, fg);
    }
    if(self->super.super.explosionState < 10)
        self->super.super.explosionState++;
}


void EnemyArr_ctor(EnemyArr* self) {
    self->size = 1;
    self->idx  = 0;
    self->arr  = calloc(self->size, sizeof(Enemy));
}

uint EnemyArr_add(EnemyArr* self, Enemy obj, MRectPtrArr* fg) {
    Enemy* prevAddr = self->arr;

    if(self->idx + 1 >= self->size) {
        self->size *= 2;

        Enemy* temp = realloc(self->arr, self->size * sizeof(Enemy));
        if(temp != NULL)
            self->arr = temp;
        else {
            free(self->arr);
            printf("\nMemory alloc problem! Aborting!\n");
            return 1;
        }
    }

    self->arr[self->idx] = obj;
    self->idx++;

    if(self->arr != prevAddr) {
        Enemy* max = prevAddr + (self->idx - 1) * sizeof(Enemy);
        for(uint i = 0; i < fg->idx; i++)
            if((MRect*)prevAddr <= fg->arr[i] && fg->arr[i] <= (MRect*)max) {
                size_t idx = ((Enemy*)fg->arr[i] - prevAddr);
                fg->arr[i] = (MRect*)&self->arr[idx];
            }
    }

    return 0;
}

void EnemyArr_update(EnemyArr* self, float td, MRectPtrArr* fg) {
    for(uint i = 0; i < self->idx; i++)
        Enemy_update(&self->arr[i], td, self, fg);
}

void EnemyArr_del(EnemyArr* self, uint id, MRectPtrArr* fg) {
    Enemy* max = self->arr + (self->idx - 1) * sizeof(Enemy);
    for(uint i = 0; i < fg->idx; i++) {
        if((MRect*)self->arr <= fg->arr[i] && fg->arr[i] <= (MRect*)max) {
            size_t idx = ((Enemy*)fg->arr[i] - self->arr);
            if(idx > id)
                fg->arr[i] = (MRect*)&self->arr[idx - 1];
        }
    }

    if(id <= self->idx + 1) {
        for(; id < self->idx - 1; id++)
            self->arr[id] = self->arr[id + 1];
        self->idx--;
    }
}