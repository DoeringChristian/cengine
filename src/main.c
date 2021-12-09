#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "mesh.h"
#include "primitives.h"
#include "cglm/cglm.h"
#include "gl_util.h"

struct light light0, light1;

int update(struct renderer *renderer, void *data){
    int time = SDL_GetTicks();

    light0.pos[0] = sinf((float)time / 1000);

    return 0;
}

int main(){
    struct window win;
    struct mesh mesh;

    window_init(&win, 0, 0, 800, 600, "test");

    //mesh_init_quad(&mesh, &win.renderer.shader);
    mesh_init_cube(&mesh);

    struct scene scene;
    scene_init(&scene);
    scene_mesh_push_back(&scene, &mesh);

    // instances
    struct ivert iv = {
        .trans = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        }
        ,0
    };
    glm_translate_make(iv.trans, vec3(0, 0, -0.6));
    glm_rotate(iv.trans, 1, vec3(1, 1, 0));
    glm_scale(iv.trans, vec3(0.2, 0.2, 0.2));

    mesh_ivert_push_back(&mesh, iv);

    glm_translate_make(iv.trans, vec3(0, -1.5, -1));
    glm_rotate(iv.trans, 0, vec3(1, 0, 0));
    glm_scale(iv.trans, vec3(1, 1, 1));

    mesh_ivert_push_back(&mesh, iv);

    glm_translate_make(iv.trans, vec3(0, 0, -3));
    glm_rotate(iv.trans, 0, vec3(1, 0, 0));
    glm_scale(iv.trans, vec3(1, 1, 1));

    mesh_ivert_push_back(&mesh, iv);

    cvert_translate(&win.renderer.camera, vec3(-0.5, -0.5, -1));
    cvert_rotate(&win.renderer.camera, vec3(1, 0, 0), 1);
    cvert_rotate(&win.renderer.camera, vec3(0, 1, 0), -0.5);
    

    // texture
    struct texture tex1;
    texture_load(&tex1, "res/img/test01.jpg");

    //mesh_texture_push(&mesh, tex1);
    mesh_texture_albedo_set(&mesh, &tex1);

    light_init(&light0, vec4(0, 1, -0.5, 1), vec4(1, 1, 1, 1), LIGHT_POINT);
    light_init(&light1, vec4(-0.4, 0, 0, 1), vec4(1, 1, 1, 0.5), LIGHT_POINT);

    scene_light_push(&scene, &light0);
    scene_light_push(&scene, &light1);

    renderer_scene_set(&win.renderer, &scene);

    window_set_update(&win, update, NULL);

    window_main(&win);

    window_free(&win);
}

