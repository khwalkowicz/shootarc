//
// Created by Kamil Walkowicz on 25/01/2018.
//

#include "engine.h"

void Game_init(Game* self, SDL_Renderer* ren) {
    MRectPtrArr_ctor(&self->fg);

    Player_ctor(&self->player, &self->fg, ren);

    EnemyArr_ctor(&self->enemies);

    // WILL REMOVE THAT IN A BIT
    Enemy_ctor(&self->enemies,  25,  980, 179, &self->fg, ren);
    Enemy_ctor(&self->enemies,  28, 1046, 179, &self->fg, ren);
    Enemy_ctor(&self->enemies,  31, 1112, 179, &self->fg, ren);
    Enemy_ctor(&self->enemies,  34, 1178, 179, &self->fg, ren);
    Enemy_ctor(&self->enemies,  37, 1244, 179, &self->fg, ren);
    Enemy_ctor(&self->enemies,  41, 1310, 179, &self->fg, ren);
    for(uint i = 0; i < self->enemies.idx; i++) {
        Enemy_addPoint(&self->enemies.arr[i], 958, 179);
        Enemy_addPoint(&self->enemies.arr[i], 937, 133);
        Enemy_addPoint(&self->enemies.arr[i], 888,  95);
        Enemy_addPoint(&self->enemies.arr[i], 829,  68);
        Enemy_addPoint(&self->enemies.arr[i], 771,  49);
        Enemy_addPoint(&self->enemies.arr[i], 710,  44);
        Enemy_addPoint(&self->enemies.arr[i], 647,  22);
        Enemy_addPoint(&self->enemies.arr[i], 527,  11);
        Enemy_addPoint(&self->enemies.arr[i], 451,   9);
        Enemy_addPoint(&self->enemies.arr[i], 394,   7);
        Enemy_addPoint(&self->enemies.arr[i], 333,   6);
        Enemy_addPoint(&self->enemies.arr[i], -88, 179);
    }

    Enemy_ctor(&self->enemies,  25,  980, 311, &self->fg, ren);
    Enemy_ctor(&self->enemies,  28, 1046, 311, &self->fg, ren);
    Enemy_ctor(&self->enemies,  31, 1112, 311, &self->fg, ren);
    Enemy_ctor(&self->enemies,  34, 1178, 311, &self->fg, ren);
    Enemy_ctor(&self->enemies,  37, 1244, 311, &self->fg, ren);
    Enemy_ctor(&self->enemies,  41, 1310, 311, &self->fg, ren);
    for(uint i = 6; i < self->enemies.idx; i++) {
        Enemy_addPoint(&self->enemies.arr[i], 958, 311);
        Enemy_addPoint(&self->enemies.arr[i], 937, 361);
        Enemy_addPoint(&self->enemies.arr[i], 888, 398);
        Enemy_addPoint(&self->enemies.arr[i], 829, 425);
        Enemy_addPoint(&self->enemies.arr[i], 771, 444);
        Enemy_addPoint(&self->enemies.arr[i], 710, 458);
        Enemy_addPoint(&self->enemies.arr[i], 647, 470);
        Enemy_addPoint(&self->enemies.arr[i], 527, 477);
        Enemy_addPoint(&self->enemies.arr[i], 451, 486);
        Enemy_addPoint(&self->enemies.arr[i], 394, 488);
        Enemy_addPoint(&self->enemies.arr[i], 333, 489);
        Enemy_addPoint(&self->enemies.arr[i], -88, 311);
    }
}

void Game_main(Game* self, Timer* timer, SDL_Renderer* ren) {
    const uint8_t* keyStates = SDL_GetKeyboardState(NULL);
    Win_controls(&self->player, &self->fg, timer, ren, keyStates);

    Rect_render((Rect*)&self->player, ren);
    Player_update(&self->player, timer->dt, &self->fg);

    EnemyArr_update(&self->enemies, timer->dt, &self->fg);

    MRectPtrArr_render(&self->fg, ren);
    MRectPtrArr_update(&self->fg, timer->dt, &self->player,
                       &self->enemies, ren);
}

void Game_clean(Game* self) {
    Player_destroy(&self->player);
    MRectPtrArr_destroy(&self->fg);
}