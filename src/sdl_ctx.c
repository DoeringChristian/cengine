#include "sdl_ctx.h"

int sdl_ctx_init(){
    if(!sdl_initialized)
        if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
            return -1;

    sdl_initialized = 1;
    return 0;
}
int sdl_ctx_deinit(){
    SDL_Quit();
    sdl_initialized = 0;
    return 0;
}
