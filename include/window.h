#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include "renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include "sdl_ctx.h"

struct window{
    SDL_Window *window;
    SDL_GLContext gl_ctx;

    int running;

    int x, y, w, h;

    struct renderer renderer;

    void *data;
    int (*update)(struct renderer *renderer, void *data);
};

int window_init(struct window *dst, int x, int y, int w, int h, const char *title);
void window_free(struct window *dst);

int window_main(struct window *dst);

void window_set_update(struct window *dst, int (*update)(struct renderer *renderer, void *data), void *data);

#endif //WINDOW_H
