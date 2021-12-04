#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include "window.h"

int main(){
    struct window win;

    window_init(&win, 0, 0, 800, 600, "test");

    window_main(&win);

    window_free(&win);
}

