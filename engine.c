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


void MainMenu_StartMenu_init(MainMenu_StartMenu* self, SDL_Renderer* ren) {
    Button_ctor(&self->startBtn,
              (SCREEN_WIDTH  - MENU_BTN_W + MENU_CURSOR_W + 10) / 2,
              (SCREEN_HEIGHT - MENU_BTN_H) / 2 + 10,
              loadTexture(ren, "menus/start_button.png"),
              MENU_BTN_W, MENU_BTN_H,
              loadTexture(ren, "menus/start_button_clicked.png")
    );
    self->freeze = 0;
    self->initialized = 1;
}

void MainMenu_StartMenu_main(MainMenu_StartMenu* self, SDL_Event event,
                             Timer* timer, MainMenu* super, SDL_Renderer* ren){
    if(SDL_PollEvent(&event)) {
        if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_SPACE ||
               event.key.keysym.sym == SDLK_RETURN) {
                Button_click(&self->startBtn);
                self->freeze = 2.5;
            }
        } else
            SDL_PushEvent(&event);
    }

    Rect_render(&self->startBtn.super, ren);

    if(self->freeze) {
        if(self->freeze <= 1) {
            super->submenu = STATE_MAINMENU_DIFFMENU;
            MainMenu_StartMenu_clean(self);
        }
        self->freeze -= timer->dt;
    }
}

void MainMenu_StartMenu_clean(MainMenu_StartMenu* self) {
    Button_destroy(&self->startBtn);
    self->initialized = 0;
}


void MainMenu_DiffMenu_init(MainMenu_DiffMenu* self, SDL_Renderer* ren) {
    Button_ctor(&self->btns[0],
                (SCREEN_WIDTH  - MENU_BTN_W + MENU_CURSOR_W + 10) / 2,
                (SCREEN_HEIGHT - MENU_BTN_H) / 2 + 10,
                loadTexture(ren, "menus/easy_btn.png"),
                MENU_BTN_W, MENU_BTN_H,
                loadTexture(ren, "menus/easy_btn_clicked.png")
    );
    Button_ctor(&self->btns[1],
                (SCREEN_WIDTH  - MENU_BTN_W + MENU_CURSOR_W + 10) / 2,
                (SCREEN_HEIGHT - MENU_BTN_H) / 2 + MENU_BTN_H + 20,
                loadTexture(ren, "menus/normal_btn.png"),
                MENU_BTN_W, MENU_BTN_H,
                loadTexture(ren, "menus/normal_btn_clicked.png")
    );
    Button_ctor(&self->btns[2],
                (SCREEN_WIDTH  - MENU_BTN_W + MENU_CURSOR_W + 10) / 2,
                (SCREEN_HEIGHT - MENU_BTN_H) / 2 + 2 * MENU_BTN_H + 30,
                loadTexture(ren, "menus/hard_btn.png"),
                MENU_BTN_W, MENU_BTN_H,
                loadTexture(ren, "menus/hard_btn_clicked.png")
    );
    self->choice = 0;
    self->freeze = 0;
    memcpy(self->options, ((const uint[]){
               PLAYER_LIFES_EASY, PLAYER_LIFES_NORMAL, PLAYER_LIFES_HARD
           }), sizeof(self->options));
    self->initialized = 1;
}

void MainMenu_DiffMenu_main(MainMenu_DiffMenu* self, SDL_Event event,
                            Timer* timer, MainMenu* super, uint* difficulty,
                            STATE* state, SDL_Renderer* ren) {
    if(SDL_PollEvent(&event)) {
        if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_SPACE ||
               event.key.keysym.sym == SDLK_RETURN) {
                Button_click(&self->btns[self->choice]);
                self->freeze = 2.5;
                *difficulty = self->options[self->choice];
            }
            if(event.key.keysym.sym == SDLK_DOWN ||
               event.key.keysym.sym == SDLK_s)
                self->choice = (self->choice + 1) % (sizeof(self->options) / sizeof(uint));
            if(event.key.keysym.sym == SDLK_UP ||
               event.key.keysym.sym == SDLK_w) {
                if(self->choice == 0)
                    self->choice = sizeof(self->options) / sizeof(uint) - 1;
                else
                    self->choice = (self->choice - 1) % (sizeof(self->options) / sizeof(uint));
            }
        } else
            SDL_PushEvent(&event);
    }

    size_t btnsLen = sizeof(self->btns)/sizeof(self->btns[0]);
    for(size_t i = 0; i < btnsLen; i++) {
        Rect_render(&self->btns[i].super, ren);
    }

    super->cursor.super.y = (SCREEN_HEIGHT - MENU_BTN_H) / 2 + 10 +
                            self->choice * (MENU_BTN_H + 10);

    if(self->freeze) {
        if(self->freeze <= 1) {
            *state = STATE_GAME;
            MainMenu_DiffMenu_clean(self);
            MainMenu_clean(super);
            self->initialized = 0;
        }
        self->freeze -= timer->dt;
    }
}

void MainMenu_DiffMenu_clean(MainMenu_DiffMenu* self) {
    size_t btnsLen = sizeof(self->btns)/sizeof(self->btns[0]);
    for(size_t i = 0; i < btnsLen; i++)
        Rect_destroy(&self->btns[i].super);
    self->initialized = 0;
}


void MainMenu_init(MainMenu* self, SDL_Renderer* ren) {
    Rect_ctor(&self->logo, "logo",
              (SCREEN_WIDTH  - LOGO_W) / 2,
              (SCREEN_HEIGHT - LOGO_H) / 2 - 105,
              LOGO_W, LOGO_H,
              loadTexture(ren, "menus/logo.png")
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

    MainMenu_StartMenu_init(&self->startMenu, ren);
    MainMenu_DiffMenu_init(&self->diffMenu,   ren);

    self->submenu = STATE_MAINMENU_STARTMENU;
    self->initialized = 1;
}

void MainMenu_main(MainMenu* self, Timer* timer, STATE* state,
                   uint* difficulty, SDL_Event event, SDL_Renderer* ren) {
    if(!self->initialized)
        MainMenu_init(self, ren);

    Rect_render(&self->logo, ren);
    Rect_render(&self->copyright, ren);

    if(self->submenu == STATE_MAINMENU_STARTMENU)
        MainMenu_StartMenu_main(&self->startMenu, event, timer, self, ren);
    if(self->submenu == STATE_MAINMENU_DIFFMENU)
        MainMenu_DiffMenu_main(&self->diffMenu, event, timer, self,
                               difficulty, state, ren);

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
    Rect_destroy(&self->cursor);
    if(self->startMenu.initialized)
        MainMenu_StartMenu_clean(&self->startMenu);
    if(self->diffMenu.initialized)
        MainMenu_DiffMenu_clean(&self->diffMenu);
    self->initialized = 0;
}


void Game_init(Game* self, uint gameContinue, SDL_Renderer* ren) {
    self->gameStopped = 0;
    self->initialized = 1;

    if(!gameContinue)
        self->levels = 0;

    self->levelsMax = LEVELS_QNT;
    self->newLevelStarted = 0;
    self->freeze = 0;

    MRectPtrArr_ctor(&self->fg);
    Player_ctor(&self->player, self->difficulty, &self->fg, ren);
    LifeBox_ctor(&self->lifeBox, &self->player, ren);

    if(self->levels < self->levelsMax) {
        if(self->levels)
            LevelFile_destroy(&self->levelFile);
        LevelFile_open(&self->levelFile, self->levelsMax);
        self->levels++;
    }

    self->level = LevelFile_read(&self->levelFile, self->levels, &self->fg, ren);
    self->newLevelStarted = 1;

    LevelScreen_init(&self->levelScreen, ren);
}

void Game_nextLevel(Game* self, STATE* state, SDL_Renderer* ren) {
    Game_clean(self);
    if(self->levels == self->levelsMax)
        *state = STATE_YOUHAVEWON;
    else
        Game_init(self, 1, ren);
}

void Game_main(Game* self, Timer* timer, STATE* state,
               SDL_Event event, SDL_Renderer* ren) {
    if(!self->initialized)
        Game_init(self, 0, ren);

    if(self->newLevelStarted) {
        self->levelScreen.opacity = 255;
        self->newLevelStarted = 0;
    }

    if(self->levelScreen.opacity)
        LevelScreen_main(&self->levelScreen, self->levels, timer->dt, ren);

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
        Level_update(&self->level, timer->dt, &self->fg);
        MRectPtrArr_update(&self->fg, timer->dt, &self->player,
                           &self->level.enemies, ren);

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

    if(Level_isFinished(&self->level) &&
       !self->freeze)
        self->freeze = 75;

    if(self->freeze) {
        if(self->freeze <= 1)
            Game_nextLevel(self, state, ren);
        else
            self->freeze -= timer->dt;
    }
}

void Game_clean(Game* self) {
    MRectPtrArr_destroy(&self->fg);
    LifeBox_destroy(&self->lifeBox);
    Player_destroy(&self->player);
    if(self->levels > 0)
        Level_destroy(&self->level);
    LevelScreen_clear(&self->levelScreen);
    LevelFile_destroy(&self->levelFile);
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


void YouHaveWonScreen_init(YouHaveWonScreen* self, SDL_Renderer* ren) {
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
              (SCREEN_WIDTH - YOUHAVEWON_TEXT_W) / 2,
              (SCREEN_HEIGHT - YOUHAVEWON_TEXT_H) / 2,
              YOUHAVEWON_TEXT_W, YOUHAVEWON_TEXT_H,
              loadTexture(ren, "menus/youhavewon.png")
    );

    self->opacity     = 25;
    self->freeze      = 25;
    self->initialized = 1;
}

void YouHaveWonScreen_main(YouHaveWonScreen* self, Timer* timer,
                         STATE* state, SDL_Renderer* ren) {
    if(!self->initialized)
        YouHaveWonScreen_init(self, ren);

    self->opacity = approach(255, self->opacity, timer->dt * 10);

    SDL_SetTextureAlphaMod(self->bg.tex,   (Uint8)self->opacity);
    SDL_SetTextureAlphaMod(self->text.tex, (Uint8)self->opacity);

    Rect_render(&self->bg, ren);
    Rect_render(&self->text, ren);

    if(self->opacity == 255)
        self->freeze -= timer->dt;

    if(self->freeze <= 0) {
        *state = STATE_MAINMENU;
        YouHaveWonScreen_clean(self);
    }
}

void YouHaveWonScreen_clean(YouHaveWonScreen* self) {
    Rect_destroy(&self->bg);
    Rect_destroy(&self->text);
    self->initialized = 0;
}