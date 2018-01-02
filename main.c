#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "config.h"
#include "functions.h"
#include "classes.h"

int main() {

    srand(time(NULL));

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        SDLError_log(stdout, "SDL_Init");
        return 1;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
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

    Player player = Player_init(ren);
    Background bg = Background_init(ren);

    float timeDelta = 0.0;
    uint  timeCurr = 0;
    uint  timePrev = 0;

    SDL_Event event;
    int gameRunning = 1;
    while(gameRunning) {
        timeCurr = SDL_GetTicks();
        timeDelta = (float)(timeCurr - timePrev) / 100;
        timePrev = timeCurr;

        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                gameRunning = 0;
            if(event.type == SDL_KEYDOWN)
                Player_move(&player, event.key.keysym.sym, 1, timeDelta);
            if(event.type == SDL_KEYUP)
                Player_move(&player, event.key.keysym.sym, 0, timeDelta);
        }

        SDL_RenderClear(ren);

        Background_render(bg, ren);
        Background_scroll(&bg, ren, timeDelta);

        Rect_render(player.super, ren);

        SDL_RenderPresent(ren);
    }

    Player_destroy(player);
    Background_destroy(bg);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 0;
}