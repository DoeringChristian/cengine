#include "window.h"

int window_init(struct window *dst, int x, int y, int w, int h, const char *title){
    //sdl_ctx_init();

    dst->x = x;
    dst->y = y;
    dst->w = w;
    dst->h = h;

    dst->window = SDL_CreateWindow(title, dst->x, dst->y, dst->w, dst->h, SDL_WINDOW_OPENGL);

    if(!dst->window)
        return -1;

    dst->gl_ctx = SDL_GL_CreateContext(dst->window);

    GLenum err = glewInit();
    if(err != GLEW_OK)
        exit(1);
    if(!GLEW_VERSION_2_1)
        exit(1);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);

    dst->running = 0;

    glViewport(0, 0, dst->w, dst->h);

    renderer_init(&dst->renderer);

    return 0;
}
void window_free(struct window *dst){
    SDL_DestroyWindow(dst->window);
    sdl_ctx_deinit();
    renderer_free(&dst->renderer);
}

int window_main(struct window *dst){

    dst->running = 1;
    while(dst->running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    dst->running = 0;
                    break;
                default:
                    break;
                }
            }
            else if(event.type == SDL_QUIT){
                dst->running = 0;
            }
        }

        renderer_render(&dst->renderer);

        SDL_GL_SwapWindow(dst->window);
    }

    return 0;
}
