#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "mesh.h"
#include "primitives.h"

int main(){
    struct window win;
    struct mesh mesh;

    window_init(&win, 0, 0, 800, 600, "test");

    mesh_init_quad(&mesh, &win.renderer.shader);

    struct scene scene;
    scene_init(&scene);
    scene_mesh_push_back(&scene, &mesh);

#if 0
    struct vert v1 = {
        .pos = {0, 0, 0},
        .normal = {0, 0, 1},
        .color = {1, 0, 0, 1},
        .uv = {0, 0},
    };
    struct vert v2 = {
        .pos = {0, 1, 0},
        .normal = {0, 0, 1},
        .color = {0, 1, 0, 1},
        .uv = {0, 1},
    };
    struct vert v3 = {
        .pos = {1, 0, 0},
        .normal = {0, 0, 1},
        .color = {0, 0, 1, 1},
        .uv = {1, 0},
    };
    mesh_vert_push_back(&mesh, v1); 
    mesh_vert_push_back(&mesh, v2);
    mesh_vert_push_back(&mesh, v3);

    mesh_tri_push_back(&mesh, tri(0, 1, 2));
#endif
    struct ivert iv;
    mat4_rotation_axis(iv.trans, (float[]){1, 0, 0}, 0);
    mesh_ivert_push_back(&mesh, iv);

#if 0
    for(size_t i = 0;i < 2;i++){
        struct ivert iv;
        mat4_rotation_axis(iv.trans, (float []){1, 0, 1}, i * M_PI / 2);
        iv.tex_idx_offset = 0;
        mesh_ivert_push_back(&mesh, iv);
    }
#endif

    renderer_push(&win.renderer);

    // texture
    struct texture tex1;
    texture_load(&tex1, "res/img/test01.jpg");

    mesh_texture_push(&mesh, tex1);

    struct lvert light0 = {
        .pos = {1, 1, 1, 1},
        .color = {0, 1, 1, 1},
    };
    struct lvert light1 = {
        .pos = {-1, -1, 1, 1},
        .color = {1, 1, 0, 1},
    };

    scene_lvert_push(&scene, light0);
    scene_lvert_push(&scene, light1);

    //renderer_push(&win.renderer);
    //mesh_push(&mesh);
    renderer_scene_set(&win.renderer, &scene);

    window_main(&win);

    window_free(&win);
}

