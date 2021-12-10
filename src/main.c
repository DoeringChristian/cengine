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
#include "loader_obj.h"

struct light light0, light1;
struct mesh mesh;
struct mesh monkey_mesh;
struct mesh cube_mesh;

int update(struct renderer *renderer, void *data){
    int time = SDL_GetTicks();

    light0.pos[0] = sinf((float)time / 1000);
    struct ivert tmp = mesh_ivert_get(&mesh, 0);
    //glm_translate(tmp.trans, vec3(0, 0, sinf((float)time / 1000)));
    //tmp.trans[3][2] = sinf((float)time/1000);
    glm_translate_make(tmp.trans, vec3(-10, -10, 10));
    //glm_translate_make(tmp.trans, vec3(0.2 * cosf((float)time/110), 0.1 * sinf((float)time/100), -0.6));
    glm_rotate(tmp.trans, 1, vec3(1, 1, 0));
    glm_scale(tmp.trans, vec3(0.2, 0.2, 0.2));
    mesh_ivert_set(&mesh, tmp, 0);

    return 0;
}

int main(){
    struct window win;

    window_init(&win, 0, 0, 800, 600, "test");

    //mesh_init_quad(&mesh, &win.renderer.shader);
    mesh_init_cube(&mesh);
    mesh_load_obj(&monkey_mesh, "res/models/monkey.obj");
    mesh_load_obj(&cube_mesh, "res/models/cube.obj");

    mesh_gen_tangent(&mesh);

    //struct scene scene;
    //scene_init(&scene);
    //scene_mesh_push_back(&scene, &mesh);

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
    glm_rotate(iv.trans, -1, vec3(1, 0, 0));
    glm_scale(iv.trans, vec3(0.2, 0.2, 0.2));

    mesh_ivert_push_back(&mesh, iv);
    mesh_ivert_push_back(&monkey_mesh, iv);
    glm_translate(iv.trans, vec3(0, 0, 10));
    mesh_ivert_push_back(&cube_mesh, iv);

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
    mesh_texture_albedo_set(&monkey_mesh, &tex1);

    light_init(&light0, vec4(0, 1, -0.5, 1), vec4(1, 1, 1, 1), LIGHT_POINT);
    light_init(&light1, vec4(-0.4, 0, 0, 1), vec4(1, 1, 1, 0.5), LIGHT_POINT);

    renderer_mesh_register(&win.renderer, &mesh);
    renderer_mesh_register(&win.renderer, &monkey_mesh);
    renderer_mesh_register(&win.renderer, &cube_mesh);
    renderer_light_register(&win.renderer, &light0);
    renderer_light_register(&win.renderer, &light1);
    

    window_set_update(&win, update, NULL);

    window_main(&win);

    window_free(&win);
}

