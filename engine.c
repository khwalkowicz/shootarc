//
// Created by Kamil Walkowicz on 25/01/2018.
//

#include "engine.h"
#include "config.h"


void pauseToggle(Timer* timer, STATE* state) {
    Timer_toggle(timer);
    if(*state == STATE_GAME)
        *state = STATE_PAUSEMENU;
    else if(*state == STATE_PAUSEMENU)
        *state = STATE_GAME;
}


void MainMenu_init(MainMenu* self, SDL_Renderer* ren) {
    Rect_ctor(&self->logo, "logo",
              (SCREEN_WIDTH  - LOGO_W) / 2,
              (SCREEN_HEIGHT - LOGO_H) / 2 - 105,
              LOGO_W, LOGO_H,
              loadTexture(ren, "menus/logo.png")
    );
    Rect_ctor(&self->startBtn, "button",
              (SCREEN_WIDTH  - MENU_BTN_W + MENU_CURSOR_W + 10) / 2,
              (SCREEN_HEIGHT - MENU_BTN_H) / 2 + 10,
              MENU_BTN_W, MENU_BTN_H,
              loadTexture(ren, "menus/start_button.png")
    );
    Rect_ctor(&self->cursor, "menu-cursor",
              (SCREEN_WIDTH  - MENU_BTN_W - MENU_CURSOR_W - 10) / 2,
              (SCREEN_HEIGHT - MENU_BTN_H) / 2 + 10,
              MENU_CURSOR_W, MENU_CURSOR_H,
              loadTexture(ren, "menus/cursor_animation.png")
    );
    self->cursorState = 0;
    Rect_ctor(&self->copyright, "text",
              (SCREEN_WIDTH  - MENU_COPYRIGHT_W) / 2,
              SCREEN_HEIGHT - MENU_COPYRIGHT_H - 5,
              MENU_COPYRIGHT_W, MENU_COPYRIGHT_H,
              loadTexture(ren, "menus/copyright.png")
    );
}

void MainMenu_main(MainMenu* self, Timer* timer, STATE* state,
                   SDL_Event event, SDL_Renderer* ren) {
    if(SDL_PollEvent(&event)) {
        if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_SPACE ||
                event.key.keysym.sym == SDLK_RETURN)
                *state = STATE_GAME;
        } else
            SDL_PushEvent(&event);
    }

    Rect_render(&self->logo, ren);
    Rect_render(&self->startBtn, ren);
    Rect_render(&self->copyright, ren);

    self->cursorState += timer->dt;
    if(self->cursorState >= 3)
        self->cursorState = 0;

    SDL_Rect clip = {
            (int)(self->cursorState * 2) * MENU_CURSOR_W, 0,
            MENU_CURSOR_W, MENU_CURSOR_H
    };
    renderTexture(self->cursor.tex, ren,
                  (int)self->cursor.super.x,
                  (int)self->cursor.super.y,
                  MENU_CURSOR_W, MENU_CURSOR_H,
                  &clip
    );
}

void MainMenu_clean(MainMenu* self) {
    Rect_destroy(&self->logo);
    Rect_destroy(&self->startBtn);
    Rect_destroy(&self->cursor);
}


void Game_init(Game* self, SDL_Renderer* ren) {
    self->difficulty = PLAYER_LIFES_NORMAL;
    self->gameStopped = 0;
    self->initialized = 1;

    MRectPtrArr_ctor(&self->fg);

    Player_ctor(&self->player, self->difficulty, &self->fg, ren);
    LifeBox_ctor(&self->lifeBox, &self->player, ren);

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

void Game_main(Game* self, Timer* timer, STATE* state,
               SDL_Event event, SDL_Renderer* ren) {
    if(!self->initialized)
        Game_init(self, ren);

    if(!self->gameStopped) {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    pauseToggle(timer, state);
            }
            else
                SDL_PushEvent(&event);
        }

        const uint8_t* keyStates = SDL_GetKeyboardState(NULL);
        Win_controls(&self->player, &self->fg, timer, ren, keyStates);

        LifeBox_update(&self->lifeBox, &self->player);
        EnemyArr_update(&self->enemies, timer->dt, &self->fg);
        MRectPtrArr_update(&self->fg, timer->dt, &self->player,
                           &self->enemies, ren);
    }

    Rect_render((Rect*)&self->player, ren);
    Player_update(&self->player, timer->dt, &self->gameStopped,
                  ren, &self->fg);

    if( (int)self->player.super.super.explosionState == 16 ) {
        *state = STATE_GAMEOVER;
        Game_clean(self);
    }

    LifeBox_render(&self->lifeBox, ren);

    MRectPtrArr_render(&self->fg, ren);
}

void Game_clean(Game* self) {
    MRectPtrArr_destroy(&self->fg);
    LifeBox_destroy(&self->lifeBox);
    Player_destroy(&self->player);
    self->initialized = 0;
}


void PauseMenu_init(PauseMenu* self, SDL_Renderer* ren) {
    Rect_ctor(&self->menu, "menu",
              (SCREEN_WIDTH  - PAUSE_MENU_W) / 2,
              (SCREEN_HEIGHT - PAUSE_MENU_H) / 2,
              PAUSE_MENU_W,
              PAUSE_MENU_H,
              loadTexture(ren, "menus/pause_menu.png")
    );
}

void PauseMenu_main(PauseMenu* self, SDL_Renderer* ren) {
    Rect_render(&self->menu, ren);
}

void PauseMenu_clean(PauseMenu* self) {
    Rect_destroy(&self->menu);
}


void GameOverScreen_init(GameOverScreen* self, SDL_Renderer* ren) {
    SDL_Rect bg = { 0, 0, SCREEN_WIDTH, SCREEN_WIDTH };
    SDL_Texture* black = SDL_CreateTexture(ren,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET,
                                           SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderTarget(ren, black);
    SDL_RenderDrawRect(ren, &bg);
    SDL_RenderFillRect(ren, &bg);
    SDL_SetRenderTarget(ren, NULL);

    Rect_ctor(&self->bg, "menu", 0, 0, SCREEN_WIDTH, SCREEN_WIDTH, black);

    Rect_ctor(&self->text, "text",
              (SCREEN_WIDTH - GAMEOVER_TEXT_W) / 2,
              (SCREEN_HEIGHT - GAMEOVER_TEXT_H) / 2,
              GAMEOVER_TEXT_W, GAMEOVER_TEXT_H,
              loadTexture(ren, "menus/gameover.png")
    );

    self->opacity     = 25;
    self->freeze      = 25;
    self->initialized = 1;
}

void GameOverScreen_main(GameOverScreen* self, Timer* timer,
                         STATE* state, SDL_Renderer* ren) {
    if(!self->initialized)
        GameOverScreen_init(self, ren);

    self->opacity = approach(255, self->opacity, timer->dt * 10);

    SDL_SetTextureAlphaMod(self->bg.tex,   (Uint8)self->opacity);
    SDL_SetTextureAlphaMod(self->text.tex, (Uint8)self->opacity);

    Rect_render(&self->bg, ren);
    Rect_render(&self->text, ren);

    if(self->opacity == 255)
        self->freeze -= timer->dt;

    if(self->freeze <= 0) {
        *state = STATE_MAINMENU;
        GameOverScreen_clean(self);
    }
}

void GameOverScreen_clean(GameOverScreen* self) {
    Rect_destroy(&self->bg);
    Rect_destroy(&self->text);
    self->initialized = 0;
}