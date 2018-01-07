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

void MShape_ctor(MShape* self, float x, float y) {
    Shape_ctor(&self->super, x, y);
    Movable_ctor(&self->vectors, 0, 0, 0, 0);
}

void MShape_update(MShape* self, float td) {
    Movable_update(&self->vectors, td);

    self->super.x += self->vectors.velocity.x * td;
    self->super.y += self->vectors.velocity.y * td;
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


void RectArr_ctor(RectArr* self) {
    self->size = 1;
    self->idx  = 0;
    self->arr  = malloc(self->size * sizeof(Rect*));
}

uint RectArr_add(RectArr* self, Rect* rectPtr) {
    if(self->idx + 1 >= self->size) {
        self->size *= 2;

        Rect** temp = realloc(self->arr, self->size * sizeof(Rect*));
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

void RectArr_del(RectArr* self, Rect* rectPtr) {
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

void RectArr_sort(RectArr* self, char towards) {
    if(towards == 'x') {
        for(uint i = 1; i < self->idx; ++i) {
            Rect* tmp = self->arr[i];
            uint j = i;
            while(j > 0 && tmp->super.x < self->arr[j - 1]->super.x) {
                self->arr[j] = self->arr[j - 1];
                --j;
            }
            self->arr[j] = tmp;
        }
    }
    if(towards == 'y') {
        for(uint i = 1; i < self->idx; ++i) {
            Rect* tmp = self->arr[i];
            uint j = i;
            while(j > 0 && tmp->super.y < self->arr[j - 1]->super.y) {
                self->arr[j] = self->arr[j - 1];
                --j;
            }
            self->arr[j] = tmp;
        }
    }
}

uint RectArr_checkCollision(RectArr* fg, Rect* obj) {
    for(uint i = 0; i < fg->idx; i++) {
        if(obj->super.x + obj->width < fg->arr[i]->super.x)
            break;
        if(fg->arr[i] != obj &&
           (obj->super.x +  obj->width >= fg->arr[i]->super.x &&
            obj->super.x <= fg->arr[i]->super.x + fg->arr[i]->width) &&
           (obj->super.y +  obj->height >= fg->arr[i]->super.y &&
            obj->super.y <= fg->arr[i]->super.y + fg->arr[i]->height)) {
            return 1;
        }
    }
    return 0;
}

void RectArr_destroy(RectArr* self) {
    free(self->arr);
}


void MRect_ctor(MRect* self, float x, float y,
                float width, float height, SDL_Texture* tex) {
    Rect_ctor(&self->super, x, y, width, height, tex);
    Movable_ctor(&self->vectors, 0, 0, 0, 0);
}

void MRect_update(MRect* self, float td, uint checkCollision, RectArr* fg) {
    Movable_update(&self->vectors, td);

    float prevX = self->super.super.x;
    float prevY = self->super.super.y;

    self->super.super.x += self->vectors.velocity.x * td;

    if(checkCollision) {
        if(Rect_checkWallsX((Rect*)self) ||
           RectArr_checkCollision(fg, (Rect*)self)) {
            self->super.super.x = prevX;
        }
    }

    self->super.super.y += self->vectors.velocity.y * td;

    if(checkCollision) {
        if(Rect_checkWallsY((Rect*)self) ||
           RectArr_checkCollision(fg, (Rect*)self)) {
            self->super.super.y = prevY;
        }
    }

    if(fg != NULL)
        RectArr_sort(fg, 'x');
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

void Player_controls(Player* self, uint8_t* keyStates) {
    /* KEY DOWN */
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

    /* KEY UP */
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

void Player_destroy(Player* self) {
    SDL_DestroyTexture(self->texUp);
    SDL_DestroyTexture(self->texDown);
    SDL_DestroyTexture(self->texNorm);
}


void Background_ctor(Background* self, float pos, SDL_Renderer* ren) {
    MRect_ctor(&self->super, pos * SCREEN_WIDTH, 0,
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
    if(self->super.super.super.x - self->super.vectors.velocity.x * td +
       SCREEN_WIDTH <= 0)
        Background_ctor(self, 1, ren);

    self->super.vectors.velocityGoal.x = win_velocity_goal;
    MRect_update((MRect*)self, td, 0, NULL);

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
