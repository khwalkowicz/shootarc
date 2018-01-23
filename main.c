#include <stdio.h>
#include <time.h>
#include <stdint.h>
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
    SDL_SetRenderDrawColor(ren, 20, 14, 35, 1);


    /* OBJECT INITS */

    MRectPtrArr fg;
    MRectPtrArr_ctor(&fg);

    Background bg0;
    Background_ctor(&bg0, 0, ren);
    Background bg1;
    Background_ctor(&bg1, 1, ren);

    Player player;
    Player_ctor(&player, &fg, ren);

    EnemyArr enemies;
    EnemyArr_ctor(&enemies);

    // WILL REMOVE THAT IN A BIT
    Enemy_ctor(&enemies,  25,  980, 179, &fg, ren);
    Enemy_ctor(&enemies,  28, 1046, 179, &fg, ren);
    Enemy_ctor(&enemies,  31, 1112, 179, &fg, ren);
    Enemy_ctor(&enemies,  34, 1178, 179, &fg, ren);
    Enemy_ctor(&enemies,  37, 1244, 179, &fg, ren);
    Enemy_ctor(&enemies,  41, 1310, 179, &fg, ren);
    for(uint i = 0; i < enemies.idx; i++) {
        Enemy_addPoint(&enemies.arr[i], 958, 179);
        Enemy_addPoint(&enemies.arr[i], 937, 133);
        Enemy_addPoint(&enemies.arr[i], 888,  95);
        Enemy_addPoint(&enemies.arr[i], 829,  68);
        Enemy_addPoint(&enemies.arr[i], 771,  49);
        Enemy_addPoint(&enemies.arr[i], 710,  44);
        Enemy_addPoint(&enemies.arr[i], 647,  22);
        Enemy_addPoint(&enemies.arr[i], 527,  11);
        Enemy_addPoint(&enemies.arr[i], 451,   9);
        Enemy_addPoint(&enemies.arr[i], 394,   7);
        Enemy_addPoint(&enemies.arr[i], 333,   6);
        Enemy_addPoint(&enemies.arr[i], -88, 179);
    }

    Enemy_ctor(&enemies,  25,  980, 311, &fg, ren);
    Enemy_ctor(&enemies,  28, 1046, 311, &fg, ren);
    Enemy_ctor(&enemies,  31, 1112, 311, &fg, ren);
    Enemy_ctor(&enemies,  34, 1178, 311, &fg, ren);
    Enemy_ctor(&enemies,  37, 1244, 311, &fg, ren);
    Enemy_ctor(&enemies,  41, 1310, 311, &fg, ren);
    for(uint i = 6; i < enemies.idx; i++) {
        Enemy_addPoint(&enemies.arr[i], 958, 311);
        Enemy_addPoint(&enemies.arr[i], 937, 361);
        Enemy_addPoint(&enemies.arr[i], 888, 398);
        Enemy_addPoint(&enemies.arr[i], 829, 425);
        Enemy_addPoint(&enemies.arr[i], 771, 444);
        Enemy_addPoint(&enemies.arr[i], 710, 458);
        Enemy_addPoint(&enemies.arr[i], 647, 470);
        Enemy_addPoint(&enemies.arr[i], 527, 477);
        Enemy_addPoint(&enemies.arr[i], 451, 486);
        Enemy_addPoint(&enemies.arr[i], 394, 488);
        Enemy_addPoint(&enemies.arr[i], 333, 489);
        Enemy_addPoint(&enemies.arr[i], -88, 311);
    }


    /* GET TIME DELTA */
    float td;
    uint  timeCurr = 0;
    uint  timePrev = 0;


    /* MAIN LOOP */

    SDL_Event event;
    uint gameRunning = 1;
    while(gameRunning) {
        timeCurr = SDL_GetTicks();
        td = (float)(timeCurr - timePrev) / 100;
        timePrev = timeCurr;

        showFPSinTitle(win, td);

        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                gameRunning = 0;
        }

        const uint8_t* keyStates = SDL_GetKeyboardState(NULL);
        Win_controls(&player, &fg, ren, keyStates);

        SDL_RenderClear(ren);

        Background_render(&bg0, ren);
        Background_update(&bg0, td, ren);
        Background_render(&bg1, ren);
        Background_update(&bg1, td, ren);

        Rect_render((Rect*)&player, ren);
        Player_update(&player, td, &fg);

        EnemyArr_update(&enemies, td, &fg);

        MRectPtrArr_render(&fg, ren);
        MRectPtrArr_update(&fg, td, &player, &enemies, ren);

        SDL_RenderPresent(ren);
    }


    /* DESTROY MALLOCS, TEXTURES AND SDL */

    Player_destroy(&player);
    MRectPtrArr_destroy(&fg);
    Background_destroy(&bg0);
    Background_destroy(&bg1);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();


    return 0;
}