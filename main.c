#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "config.h"
#include "window.h"
#include "engine.h"

int main() {

    srand((uint)time(NULL));


    /* SDL INITS */

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        SDLError_log(stdout, "SDL_Init");
        return 1;
    }

    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDLError_log(stdout, "IMG_Init");
        SDL_Quit();
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow(WINDOW_TITLE,
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       SCREEN_WIDTH, SCREEN_HEIGHT,
                                       SDL_WINDOW_SHOWN);
    if(win == NULL) {
        SDLError_log(stdout, "SDL_CreateWindow");
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1,
                                           SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_PRESENTVSYNC);
    if(ren == NULL) {
        SDL_DestroyWindow(win);
        SDLError_log(stdout, "SDL_CreateRenderer");
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderDrawColor(ren, 20, 14, 35, 1);


    /* OBJECT INITS */

    Background bg0;
    Background_ctor(&bg0, 0, ren);
    Background bg1;
    Background_ctor(&bg1, 1, ren);

    // INIT GAME - Game_init(ren);
    Game game;
    Game_init(&game, ren);

    /* START THE TIMER */
    Timer timer;
    Timer_ctor(&timer);


    /* MAIN LOOP */

    SDL_Event event;
    uint gameRunning = 1;
    while(gameRunning) {
        Timer_update(&timer);

        showFPSinTitle(win, timer.dt);

        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                gameRunning = 0;
            if(event.type == SDL_KEYDOWN)
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    Timer_toggle(&timer);
        }

        SDL_RenderClear(ren);

        Background_render(&bg0, ren);
        Background_update(&bg0, timer.dt, ren);
        Background_render(&bg1, ren);
        Background_update(&bg1, timer.dt, ren);


        // GAME MAIN LOGIC - Game_main()
        Game_main(&game, &timer, ren);

        SDL_RenderPresent(ren);
    }


    /* DESTROY MALLOCS, TEXTURES AND SDL */

    // GAME CLEANUP - Game_clean()
    Game_clean(&game);

    Background_destroy(&bg0);
    Background_destroy(&bg1);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();


    return 0;
}