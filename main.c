#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "config.h"
#include "window.h"

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
    SDL_SetRenderDrawColor(ren, 9, 0, 22, 1);


    /* OBJECT INITS */

    float win_velocity_goal = WIN_VELOCITY;

    Player player;
    Player_ctor(&player, ren);

    Background bg0;
    Background_ctor(&bg0, 0, ren);
    Background bg1;
    Background_ctor(&bg1, 1, ren);


    /* GET TIME DELTA */
    float td;
    uint  timeCurr = 0;
    uint  timePrev = 0;


    /* MAIN LOOP */

    SDL_Event event;
    int gameRunning = 1;
    while(gameRunning) {
        timeCurr = SDL_GetTicks();
        td = (float)(timeCurr - timePrev) / 100;
        timePrev = timeCurr;

        showFPSinTitle(win, td);

        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                gameRunning = 0;
            if(event.type == SDL_KEYDOWN)
                Win_controls(&win_velocity_goal, &player, event.key.keysym.sym, 1);
            if(event.type == SDL_KEYUP)
                Win_controls(&win_velocity_goal, &player, event.key.keysym.sym, 0);
        }

        SDL_RenderClear(ren);

        Background_render(&bg0, ren);
        Background_update(&bg0, win_velocity_goal, td, ren);
        Background_render(&bg1, ren);
        Background_update(&bg1, win_velocity_goal, td, ren);

        Rect_render((Rect*)&player, ren);
        MRect_update((MRect*)&player, td); // !!!!

        SDL_RenderPresent(ren);
    }


    /* DESTROY TEXTURES AND KILL SDL */

    Player_destroy(&player);
    Background_destroy(&bg0);
    Background_destroy(&bg1);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();


    return 0;
}