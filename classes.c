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

void Movable_ctor(Movable* self, float velX, float velY,
                  float velGoalX, float velGoalY) {
    Vector_ctor(&self->velocity, velX, velY);
    Vector_ctor(&self->velocityGoal, velGoalX, velGoalY);
}

void Movable_update(Movable* self, float td) {
    self->velocity.x = approach(self->velocityGoal.x, self->velocity.x, td * 50);
    self->velocity.y = approach(self->velocityGoal.y, self->velocity.y, td * 50);
}


void Rect_ctor(Rect* self, char* type, float x, float y,
               float width, float height, SDL_Texture* tex){
    Shape_ctor(&self->super, x, y);

    self->type   = type;
    self->height = height;
    self->width  = width;
    self->tex    = tex;
    self->explosionState = 0;
}

void Rect_render(Rect* self, SDL_Renderer* ren) {
    SDL_Rect dest;
    dest.x = (int)self->super.x;
    dest.y = (int)self->super.y;
    dest.w = (int)self->width;
    dest.h = (int)self->height;
    SDL_RenderCopy(ren, self->tex, NULL, &dest);
}

uint Rect_checkWallsX(Rect* self) {
    if(self->super.x <= 0 || self->super.x + self->width >= SCREEN_WIDTH)
        return 1;
    return 0;
}

uint Rect_checkWallsY(Rect* self) {
    if(self->super.y <= 0 || self->super.y + self->height >= SCREEN_HEIGHT)
        return 1;
    return 0;
}

void Rect_destroy(Rect* self) {
    SDL_DestroyTexture(self->tex);
}


void MRect_ctor(MRect* self, char* type, float x, float y,
                float width, float height, SDL_Texture* tex) {
    Rect_ctor(&self->super, type, x, y, width, height, tex);
    Movable_ctor(&self->vectors, 0, 0, 0, 0);
}

int MRect_update(MRect* self, float td, uint checkCollision, Player* player,
                 EnemyArr* enemies, MRectPtrArr* fg, SDL_Renderer* ren) {
    Movable_update(&self->vectors, td);

    float prevX = self->super.super.x;
    float prevY = self->super.super.y;

    self->super.super.x += self->vectors.velocity.x * td;

    if(checkCollision) {
        MRect* coll = MRectPtrArr_checkCollision(fg, self);
        if(coll != NULL || ( strcmp(self->super.type, "enemy") != 0 &&
                             Rect_checkWallsX((Rect*)self) )) {
            self->super.super.x = prevX;
            if( !strcmp(self->super.type, "player_shot") ) {
                Player_handleShot(player, fg, self, coll);
                return 0;
            }
        }
    }

    self->super.super.y += self->vectors.velocity.y * td;

    if(checkCollision) {
        MRect* coll = MRectPtrArr_checkCollision(fg, self);
        if(coll != NULL || ( strcmp(self->super.type, "enemy") != 0 &&
                             Rect_checkWallsY((Rect*)self) )) {
            self->super.super.y = prevY;
            if( !strcmp(self->super.type, "player_shot") ) {
                Player_handleShot(player, fg, self, coll);
                return 0;
            }
        }
    }

    if(enemies != NULL && self->super.explosionState)
        Enemy_explode((Enemy*)self, enemies, fg, ren);

    if(fg != NULL)
        MRectPtrArr_sort(fg, 'x');
    return 0;
}


void MRectArr_ctor(MRectArr* self) {
    self->size = 1;
    self->idx  = 0;
    self->arr  = calloc(self->size, sizeof(MRect));
}

uint MRectArr_add(MRectArr* self, MRect obj) {
    if(self->idx + 1 >= self->size) {
        self->size *= 2;

        MRect* temp = realloc(self->arr, self->size * sizeof(MRect));
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

    return 0;
}

void MRectArr_del(MRectArr* self, uint id) {
    if(id <= self->idx + 1) {
        for(; id < self->idx - 1; id++)
            self->arr[id] = self->arr[id + 1];
        self->idx--;
    }
}


void MRectPtrArr_ctor(MRectPtrArr* self) {
    self->size = 1;
    self->idx  = 0;
    self->arr  = calloc(self->size, sizeof(Rect*));
}

uint MRectPtrArr_add(MRectPtrArr* self, MRect* rectPtr) {
    if(self->idx + 1 >= self->size) {
        self->size *= 2;

        MRect** temp = realloc(self->arr, self->size * sizeof(MRect*));
        if(temp != NULL)
            self->arr = temp;
        else {
            free(self->arr);
            printf("\nMemory alloc problem! Aborting!\n");
            return 1;
        }
    }

    self->arr[self->idx] = rectPtr;
    self->idx++;

    return 0;
}

void MRectPtrArr_del(MRectPtrArr* self, MRect* rectPtr) {
    uint i = 0;
    for(; i < self->idx; i++)
        if(self->arr[i] == rectPtr)
            break;
    if(i != self->idx + 1) {
        for(; i < self->idx - 1; i++)
            self->arr[i] = self->arr[i + 1];
        self->idx--;
    }
}

void MRectPtrArr_sort(MRectPtrArr* self, char towards) {
    if(towards == 'x') {
        for(uint i = 1; i < self->idx; ++i) {
            MRect* tmp = self->arr[i];
            uint j = i;
            while(j > 0 && tmp->super.super.x <
                                   self->arr[j - 1]->super.super.x) {
                self->arr[j] = self->arr[j - 1];
                --j;
            }
            self->arr[j] = tmp;
        }
    }
    if(towards == 'y') {
        for(uint i = 1; i < self->idx; ++i) {
            MRect* tmp = self->arr[i];
            uint j = i;
            while(j > 0 && tmp->super.super.y <
                                   self->arr[j - 1]->super.super.y) {
                self->arr[j] = self->arr[j - 1];
                --j;
            }
            self->arr[j] = tmp;
        }
    }
}

MRect* MRectPtrArr_checkCollision(MRectPtrArr* self, MRect* obj) {
    for(uint i = 0; i < self->idx; i++) {
        if(obj->super.super.x + obj->super.width < self->arr[i]->super.super.x)
            break;
        if(self->arr[i] != obj &&
           (obj->super.super.x +  obj->super.width >=
                    self->arr[i]->super.super.x &&
            obj->super.super.x <=
                    self->arr[i]->super.super.x + self->arr[i]->super.width) &&
           (obj->super.super.y +  obj->super.height >=
                    self->arr[i]->super.super.y &&
            obj->super.super.y <=
                    self->arr[i]->super.super.y + self->arr[i]->super.height))
            return self->arr[i];
    }
    return NULL;
}

void MRectPtrArr_render(MRectPtrArr* self, SDL_Renderer* ren) {
    for(uint i = 0; i < self->idx; i++)
        if( !strcmp(self->arr[i]->super.type, "player_shot") ||
            !strcmp(self->arr[i]->super.type, "enemy") )
            Rect_render((Rect*)self->arr[i], ren);
}

void MRectPtrArr_update(MRectPtrArr* self, float td, Player* player,
                        EnemyArr* enemies, SDL_Renderer* ren) {
    for(uint i = 0; i < self->idx; i++)
        if( !strcmp(self->arr[i]->super.type, "player_shot") ||
            !strcmp(self->arr[i]->super.type, "enemy") )
            MRect_update(self->arr[i], td, 1, player, enemies, self, ren);
}

void MRectPtrArr_destroy(MRectPtrArr* self) {
    for(uint i = 0; i < self->idx; i++)
        if( strcmp(self->arr[i]->super.type, "player") != 0)
            Rect_destroy((Rect*)self->arr[i]);
    free(self->arr);
}


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
    size_t idx = obj - self->shots.arr;
    MRectPtrArr_del(fg, obj);
    MRectArr_del(&self->shots, (uint)idx);
}

void Player_destroy(Player* self) {
    SDL_DestroyTexture(self->texUp);
    SDL_DestroyTexture(self->texDown);
    SDL_DestroyTexture(self->texNorm);
}


void Background_ctor(Background* self, float pos, SDL_Renderer* ren) {
    MRect_ctor(&self->super, "background",
               pos * SCREEN_WIDTH, 0,
               SCREEN_WIDTH, SCREEN_HEIGHT, NULL);

    self->super.vectors.velocity.x = WIN_VELOCITY;
    self->super.vectors.velocityGoal.x = WIN_VELOCITY;

    self->tiles_x = (uint)ceil((double)self->super.super.width / BG_TILE_W);
    self->tiles_y = (uint)ceil((double)self->super.super.height / BG_TILE_H);
    self->size = self->tiles_x * self->tiles_y;
    self->array = malloc(self->size * sizeof(Rect));

    for(uint h_i = 0; h_i < self->tiles_y; h_i++)
        for(uint w_i = 0; w_i < self->tiles_x; w_i++) {

            int id = rand() % BG_TILES_Q + 1;
            char* path = malloc(strlen(BG_TILES_PATH) + 2 + 4);
            sprintf(path, "%s%d.png", BG_TILES_PATH, id);

            Rect tile;
            Rect_ctor(&tile,
                      "bg_tile",
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

void Background_update(Background* self, float td, SDL_Renderer* ren) {
    if(self->super.super.super.x - self->super.vectors.velocity.x * td +
       SCREEN_WIDTH <= 0)
        Background_ctor(self, 1, ren);

    self->super.vectors.velocityGoal.x = WIN_VELOCITY;
    MRect_update((MRect*)self, td, 0, NULL, NULL, NULL, NULL);

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
