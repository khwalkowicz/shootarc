//
// Created by Kamil Walkowicz on 29/01/2018.
//

#include "levels.h"
#include "window.h"

void Level_ctor(Level* self, uint* levels,
                MRectPtrArr* fg, SDL_Renderer* ren) {
    (*levels)++;
    self->number = *levels;



    EnemyArr_ctor(&self->enemies);

    // WILL REMOVE THAT IN A BIT
    Enemy_ctor(&self->enemies,  25,  980, 179, fg, ren);
    Enemy_ctor(&self->enemies,  28, 1046, 179, fg, ren);
    Enemy_ctor(&self->enemies,  31, 1112, 179, fg, ren);
    Enemy_ctor(&self->enemies,  34, 1178, 179, fg, ren);
    Enemy_ctor(&self->enemies,  37, 1244, 179, fg, ren);
    Enemy_ctor(&self->enemies,  41, 1310, 179, fg, ren);
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

    Enemy_ctor(&self->enemies,  25,  980, 311, fg, ren);
    Enemy_ctor(&self->enemies,  28, 1046, 311, fg, ren);
    Enemy_ctor(&self->enemies,  31, 1112, 311, fg, ren);
    Enemy_ctor(&self->enemies,  34, 1178, 311, fg, ren);
    Enemy_ctor(&self->enemies,  37, 1244, 311, fg, ren);
    Enemy_ctor(&self->enemies,  41, 1310, 311, fg, ren);
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

void Level_update(Level* self, float dt, MRectPtrArr* fg) {
    EnemyArr_update(&self->enemies, dt, fg);
}

uint Level_isFinished(Level* self) {
    if(self->enemies.size > 1 && self->enemies.idx == 0)
        return 1;
    return 0;
}

void Level_destroy(Level* self) {
    free(self->enemies.arr);
}


void LevelScreen_init(LevelScreen* self, SDL_Renderer* ren) {
    memcpy(NUMBERS_W, ((const uint[]){
            15, 9, 15, 15, 18, 15, 15, 18, 17, 14
    }), sizeof(NUMBERS_W));

    Rect_ctor(&self->bg, "bg",
              (SCREEN_WIDTH  - PAUSE_MENU_W) / 2,
              (SCREEN_HEIGHT - PAUSE_MENU_H) / 2,
              PAUSE_MENU_W, PAUSE_MENU_H,
              loadTexture(ren, "ui/levelscreen_bg.png")
    );
    Rect_ctor(&self->text, "text",
              (SCREEN_WIDTH  - LEVEL_TEXT_W) / 2 - 7,
              (SCREEN_HEIGHT - LEVEL_TEXT_H) / 2,
              LEVEL_TEXT_W, LEVEL_TEXT_H,
              loadTexture(ren, "ui/levelscreen_txt.png")
    );
    for(uint i = 0; i < 10; i++) {
        char path[18];
        snprintf(path, 17, "ui/numbers/%u.png", i);
        self->numbers[i] = loadTexture(ren, path);
    }
    self->opacity = 0;
}

void LevelScreen_main(LevelScreen* self, uint number,
                      double dt, SDL_Renderer* ren) {
    MRectArr_ctor(&self->numbersCurr);
    self->text.super.x = (SCREEN_WIDTH  - LEVEL_TEXT_W) / 2 - 7;

    uint temp = number;
    uint numberLen = 0;
    while(temp) {
        temp /= 10;
        numberLen++;
    }

    temp = number;
    while(temp) {
        uint character = temp % 10;
        temp /= 10;

        MRect obj;
        MRect_ctor(&obj, "text",
                   (SCREEN_WIDTH + LEVEL_TEXT_W) / 2 + 7 +
                       (NUMBERS_W[character] + 6) *
                               (numberLen - self->numbersCurr.idx - 1),
                   (SCREEN_HEIGHT - NUMBERS_H) / 2,
                   NUMBERS_W[character], NUMBERS_H,
                   self->numbers[character]
        );
        MRectArr_add(&self->numbersCurr, obj);

        self->text.super.x -= (NUMBERS_W[character] + 2) / 2;
    }

    if(self->opacity > 0)
        self->opacity = approach(0, self->opacity, dt * 10);
    if(self->opacity < 0)
        self->opacity = 0;

    SDL_SetTextureAlphaMod(self->bg.tex,   (Uint8)self->opacity);
    SDL_SetTextureAlphaMod(self->text.tex, (Uint8)self->opacity);
    for(int i = self->numbersCurr.idx - 1; i >= 0; i--)
        SDL_SetTextureAlphaMod(self->numbersCurr.arr[i].super.tex,
                               (Uint8)self->opacity);

    Rect_render(&self->bg, ren);
    Rect_render(&self->text, ren);
    for(int i = self->numbersCurr.idx - 1; i >= 0; i--) {
        Rect_render((Rect*)&self->numbersCurr.arr[i], ren);
        MRectArr_del(&self->numbersCurr, (uint)i);
    }

    free(self->numbersCurr.arr);
}

void LevelScreen_clear(LevelScreen* self) {
    Rect_destroy(&self->bg);
    Rect_destroy(&self->text);
    for(uint i = 0; i < 10; i++)
        SDL_DestroyTexture(self->numbers[i]);
}
