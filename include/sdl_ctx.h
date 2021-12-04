#ifndef SDL_CTX_H
#define SDL_CTX_H

#include <SDL2/SDL.h>

static int sdl_initialized = 0;

int sdl_ctx_init();
int sdl_ctx_deinit();

#endif //SDL_CTX_H
