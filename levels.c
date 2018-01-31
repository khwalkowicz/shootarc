//
// Created by Kamil Walkowicz on 29/01/2018.
//

#include "levels.h"
#include "window.h"

const char* getfield(char* line, int num) {
    char* tmp = strdup(line);
    const char* tok;
    for (tok = strtok(tmp, ",");
         tok && *tok;
         tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

const uint getlen(char* line) {
    char* tmp = strdup(line);
    uint counter = 0;
    char* tok;
    tok = strtok(tmp, ",");
    while(tok != NULL) {
        counter++;
        tok = strtok(NULL, ",");
    }
    return counter;
}


void Level_ctor(Level* self, uint* levels) {
    (*levels)++;
    self->number = *levels;

    EnemyArr_ctor(&self->enemies);
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


uint LevelFile_open(LevelFile* self, uint number) {
    uint temp = number;
    uint numberLen = 0;
    while(temp) {
        temp /= 10;
        numberLen++;
    }

    char* path = malloc(sizeof(LEVELSPATH) + (numberLen + 1) * sizeof(char));
    sprintf(path, "%s%u", LEVELSPATH, number);

    FILE* levelFile = fopen(path, "r+");
    if(!levelFile) {
        fprintf(stderr, "\nLevel_read: Unable to open file %s . Aborting.\n",
                path);
        return 1;
    }
    self->levelFile = levelFile;
    return 0;
}

Level LevelFile_read(LevelFile* self, uint* levels,
                     MRectPtrArr* fg, SDL_Renderer* ren) {
    Level new;
    Level_ctor(&new, levels);

    char line[1024];
    while(fgets(line, 1024, self->levelFile)) {
        uint len = getlen(line);
        Enemy_ctor(&new.enemies,
                   (float)strtod(getfield(line, 1), NULL),
                   (float)strtod(getfield(line, 2), NULL),
                   (float)strtod(getfield(line, 3), NULL),
                   fg, ren
        );
        for(uint i = 4; i < len - 1; i = i + 2)
            Enemy_addPoint(&new.enemies.arr[new.enemies.idx - 1],
                           (float)strtod(getfield(line, i), NULL),
                           (float)strtod(getfield(line, i + 1), NULL)
            );
    }

    rewind(self->levelFile);
    return new;
}

void LevelFile_destroy(LevelFile* self) {
    fclose(self->levelFile);
}