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

uint MRect_handleCollisionX(MRect* self, MRect* coll, float prev,
                            Player* player, EnemyArr* enemies,
                            MRectPtrArr* fg, SDL_Renderer* ren) {
    if(!strcmp(self->super.type, "player")) {
        if(Rect_checkWallsX((Rect*)self)) {
            self->super.super.x = prev;
            return 0;
        }
        if(coll != NULL) {
            if(!strcmp(coll->super.type, "enemy") &&
                    !coll->super.explosionState) {
                Enemy_explode((Enemy*)coll, enemies, fg, ren);
                Player_hit(player);
                return 1;
            }
            if(!strcmp(coll->super.type, "rock") &&
                    !coll->super.explosionState) {
                // Rock_explode() - will add later
                Player_hit(player);
                return 1;
            }
        }
        return 0;
    }
    if(!strcmp(self->super.type, "player_shot")) {
        if(Rect_checkWallsX((Rect*)self)) {
            Player_handleShot(player, fg, self, coll);
            return 1;
        }
        if(coll != NULL) {
            if(!strcmp(coll->super.type, "enemy")) {
                Player_handleShot(player, fg, self, coll);
                return 1;
            }
            if(!strcmp(coll->super.type, "rock")) {
                Player_handleShot(player, fg, self, coll);
                return 1;
            }
        }
    }
    if(!strcmp(self->super.type, "enemy")) {
        if(coll != NULL) {
            if(!strcmp(coll->super.type, "player") &&
                    !self->super.explosionState) {
                Enemy_explode((Enemy*)self, enemies, fg, ren);
                Player_hit(player);
                return 1;
            }
            if(!strcmp(coll->super.type, "player_shot")) {
                Player_handleShot(player, fg, coll, self);
                return 1;
            }
            if(!strcmp(coll->super.type, "enemy")) {
                self->super.super.x = prev;
                return 0;
            }
            if(!strcmp(coll->super.type, "rock") &&
                    !coll->super.explosionState &&
                    !self->super.explosionState) {
                // Rock_explode() - will add later
                Enemy_explode((Enemy*)self, enemies, fg, ren);
                return 1;
            }
        }
        return 0;
    }
    if(!strcmp(self->super.type, "rock")) {
        if(coll != NULL) {
            if(!strcmp(coll->super.type, "player") &&
                    !self->super.explosionState) {
                // Rock_explode() - will add later
                Player_hit(player);
                return 1;
            }
            if(!strcmp(coll->super.type, "player_shot")) {
                Player_handleShot(player, fg, coll, self);
                return 1;
            }
            if(!strcmp(coll->super.type, "enemy") &&
                    !self->super.explosionState &&
                    !coll->super.explosionState) {
                // Rock_explode() - will add later
                Enemy_explode((Enemy*)coll, enemies, fg, ren);
                return 1;
            }
            if(!strcmp(coll->super.type, "rock") &&
                    !self->super.explosionState &&
                    !coll->super.explosionState) {
                // Rock_explode( self ) - will add later
                // Rock_explode( coll ) - will add later
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

uint MRect_handleCollisionY(MRect* self, MRect* coll, float prev,
                            Player* player, EnemyArr* enemies,
                            MRectPtrArr* fg, SDL_Renderer* ren) {
    if(!strcmp(self->super.type, "player")) {
        if(Rect_checkWallsY((Rect*)self)) {
            self->super.super.y = prev;
            return 0;
        }
        if(coll != NULL) {
            if(!strcmp(coll->super.type, "enemy") &&
               !coll->super.explosionState) {
                Enemy_explode((Enemy*)coll, enemies, fg, ren);
                Player_hit(player);
                return 1;
            }
            if(!strcmp(coll->super.type, "rock") &&
               !coll->super.explosionState) {
                // Rock_explode() - will add later
                Player_hit(player);
                return 1;
            }
        }
        return 0;
    }
    if(!strcmp(self->super.type, "player_shot")) {
        if(Rect_checkWallsY((Rect*)self)) {
            Player_handleShot(player, fg, self, coll);
            return 1;
        }
        if(coll != NULL) {
            if(!strcmp(coll->super.type, "enemy")) {
                Player_handleShot(player, fg, self, coll);
                return 1;
            }
            if(!strcmp(coll->super.type, "rock")) {
                Player_handleShot(player, fg, self, coll);
                return 1;
            }
        }
    }
    if(!strcmp(self->super.type, "enemy")) {
        if(coll != NULL) {
            if(!strcmp(coll->super.type, "player") &&
               !self->super.explosionState) {
                Enemy_explode((Enemy*)self, enemies, fg, ren);
                Player_hit(player);
                return 1;
            }
            if(!strcmp(coll->super.type, "player_shot")) {
                Player_handleShot(player, fg, coll, self);
                return 1;
            }
            if(!strcmp(coll->super.type, "enemy")) {
                self->super.super.y = prev;
                return 0;
            }
            if(!strcmp(coll->super.type, "rock") &&
               !coll->super.explosionState &&
               !self->super.explosionState) {
                // Rock_explode() - will add later
                Enemy_explode((Enemy*)self, enemies, fg, ren);
                return 1;
            }
        }
        return 0;
    }
    if(!strcmp(self->super.type, "rock")) {
        if(coll != NULL) {
            if(!strcmp(coll->super.type, "player") &&
               !self->super.explosionState) {
                // Rock_explode() - will add later
                Player_hit(player);
                return 1;
            }
            if(!strcmp(coll->super.type, "player_shot")) {
                Player_handleShot(player, fg, coll, self);
                return 1;
            }
            if(!strcmp(coll->super.type, "enemy") &&
               !self->super.explosionState &&
               !coll->super.explosionState) {
                // Rock_explode() - will add later
                Enemy_explode((Enemy*)coll, enemies, fg, ren);
                return 1;
            }
            if(!strcmp(coll->super.type, "rock") &&
               !self->super.explosionState &&
               !coll->super.explosionState) {
                // Rock_explode( self ) - will add later
                // Rock_explode( coll ) - will add later
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

int MRect_update(MRect* self, float td, uint checkCollision, Player* player,
                 EnemyArr* enemies, MRectPtrArr* fg, SDL_Renderer* ren) {
    Movable_update(&self->vectors, td);

    float prevX = self->super.super.x;
    float prevY = self->super.super.y;

    self->super.super.x += self->vectors.velocity.x * td;

    uint wasErased = 0;

    if(checkCollision) {
        MRect* coll = MRectPtrArr_checkCollision(fg, self);
        wasErased = MRect_handleCollisionX(self, coll, prevX, player, enemies,
                                           fg, ren);
    }

    self->super.super.y += self->vectors.velocity.y * td;

    if(checkCollision && !wasErased) {
        MRect* coll = MRectPtrArr_checkCollision(fg, self);
        MRect_handleCollisionY(self, coll, prevY, player, enemies, fg, ren);
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
    if(self->idx) {
        uint i = 0;
        for (; i < self->idx; i++)
            if (self->arr[i] == rectPtr)
                break;
        if (i != self->idx + 1) {
            for (; i < self->idx - 1; i++)
                self->arr[i] = self->arr[i + 1];
            self->idx--;
        }
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
        MRect_update(self->arr[i], td, 1, player, enemies, self, ren);
}

void MRectPtrArr_destroy(MRectPtrArr* self) {
    for(uint i = 0; i < self->idx; i++) {
        if(strcmp(self->arr[i]->super.type, "player") != 0)
            Rect_destroy((Rect*)self->arr[i]);
    }
    free(self->arr);
}


void Button_ctor(Button* self, float x, float y, SDL_Texture* tex,
                 float width, float height, SDL_Texture* clicked) {
    Rect_ctor(&self->super, "button", x, y, width, height, tex);
    self->clicked = clicked;
}

void Button_click(Button* self) {
    SDL_Texture* temp = self->super.tex;
    self->super.tex = self->clicked;
    self->clicked = temp;
}

void Button_destroy(Button* self) {
    Rect_destroy(&self->super);
    SDL_DestroyTexture(self->clicked);
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
