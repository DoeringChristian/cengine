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
#include "material.h"

struct light light0, light1;
struct mesh mesh;
struct mesh monkey_mesh;
struct mesh cube_mesh;
int time;

int update(struct renderer *renderer, void *data){
    int time_tmp = SDL_GetTicks();
    //printf("FrameTime: %i ms FPS: %f\n", time_tmp - time, 1000/((float) time_tmp - (float) time));
    time = time_tmp;


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

    window_init(&win, 0, 0, 1000, 1000, "test");

    //mesh_init_quad(&mesh, &win.renderer.shader);
    mesh_init_cube(&mesh);
    mesh_load_obj(&monkey_mesh, "res/models/monkey.obj");
    mesh_load_obj(&cube_mesh, "res/models/cube.obj");
    //monkey_mesh.has_shadow = 0;

    mesh_gen_tangent(&mesh);
    mesh_gen_tangent(&monkey_mesh);

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
    //glm_scale(iv.trans, vec3(3, 3, 3));
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
    struct material material0, material1;
    material_init(&material0);
    material_init(&material1);

    struct texture tex1, tex1_normal;
    texture_load(&tex1, "res/img/test01.jpg");
    texture_load(&tex1_normal, "res/img/test01_normal.jpg");

    struct texture tex2, tex2_normal, tex2_mrao;
    texture_load(&tex2, "res/img/rustediron2_basecolor.png");
    texture_load(&tex2_normal, "res/img/rustediron2_normal.png");
    texture_load(&tex2_mrao, "res/img/rustediron2_mrao.png");
    
    material_map_albedo_set(&material0, &tex1);
    material_map_normal_set(&material0, &tex1_normal);

    material_map_albedo_set(&material1, &tex2);
    material_map_mrao_set(&material1, &tex2_mrao);
    material_map_normal_set(&material1, &tex2_normal);
    glm_vec4_copy(vec4(0, 1, 1, 0), material1.mrao);
    glm_vec4_copy(vec4(1, 1, 1, 1), material1.emission);


    //mesh_texture_push(&mesh, tex1);
#if 0
    mesh_texture_albedo_set(&mesh, &tex1);
    mesh_texture_normal_set(&mesh, &tex1_normal);
    mesh_texture_albedo_set(&monkey_mesh, &tex1);
#endif
    mesh_material_set(&mesh, &material0);
    mesh_material_set(&monkey_mesh, &material1);
    //mesh_texture_normal_set(&monkey_mesh, &tex1_normal);

    light_init(&light0, vec4(0, 1, -0.5, 1), vec4(1, 1, 1, 1), LIGHT_POINT);
    light_init(&light1, vec4(-0.4, 0, 0, 1), vec4(1, 1, 1, 0.5), LIGHT_POINT);

    renderer_mesh_register(&win.renderer, &mesh);
    renderer_mesh_register(&win.renderer, &monkey_mesh);
    renderer_mesh_register(&win.renderer, &cube_mesh);
    renderer_light_register(&win.renderer, &light0);
    renderer_light_register(&win.renderer, &light1);
    
    //glm_vec4_copy(vec4(1, 1, 1, 1), material1.emission);

    window_set_update(&win, update, NULL);

    window_main(&win);

    window_free(&win);
}

