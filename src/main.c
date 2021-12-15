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
#include "loader_gltf.h"
#include "material.h"
#include "container.h"
#include "instancer.h"

struct container c1;
struct instancer inst;
int time;

int update(struct renderer *renderer, void *data){

    struct mesh *suzanne = container_mesh_search(&c1, "Suzanne");

    int time_tmp = SDL_GetTicks();
    //printf("FrameTime: %i ms FPS: %f\n", time_tmp - time, 1000/((float) time_tmp - (float) time));
    time = time_tmp;


    int mx, my;
    mat4 cr;
    SDL_PumpEvents();
    SDL_GetMouseState(&mx, &my);

    glm_mat4_identity(cr);
    glm_translate(cr, vec3(0, 0, -1));

    glm_rotate(cr, (float)mx / 100, vec3(0, 1, 0));
    glm_rotate(cr, -(float)my / 100, vec3(1, 0, 0));

    glm_translate(cr, vec3(0, 0, 0.6));
    glm_mat4_copy(cr, renderer->camera.view);

    instancer_update(&inst);

    return 0;
}

int main(){
    struct window win;
    window_init(&win, 0, 0, 1000, 1000, "test");

    instancer_init(&inst);


    struct mesh mesh;
    mesh_init_cube(&mesh);

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

    mesh_ivert_push_back(&mesh, ivert(
                vec3(0, 0, -0.6), 
                -1, vec3(1, 0, 0), 
                vec3(0.2, 0.2, 0.2), 
                vec2(0, 0)));

    //renderer_mesh_register(&win.renderer, &mesh);

    struct material material0 = {0};


    struct texture tex1, tex1_normal, tex1_mrao;
    texture_load_m(&tex1, "res/img/wood_table/wood_table_001_diff_4k.jpg", -0.4);
    texture_load_m(&tex1_normal, "res/img/wood_table/wood_table_001_nor_gl_4k.jpg", -0.4);
    texture_load_m(&tex1_mrao, "res/img/wood_table/wood_table_001_mra_4k.jpg", -0.4);

    

    material_albedo_map_set(&material0, &tex1);
    material_normal_map_set(&material0, &tex1_normal, 0.01);
    material_mrao_map_set(&material0, &tex1_normal);

    mesh_material_set(&mesh, &material0);

    mesh_cull_from_normal(&mesh);
    mesh_gen_tangent(&mesh);






    container_init(&c1);
    container_load_obj(&c1, "res/models/monkey.obj");

    struct mesh *suzanne = container_mesh_search(&c1, "Suzanne");

#if 0
    mesh_ivert_push_back(suzanne, 
            ivert(vec3(0, 0, -0.6),
                -1, vec3(1, 0, 0), 
                vec3(0.2, 0.2, 0.2),
                vec2(0, 0)));
#endif
    struct instance ins0;
    instance_init(&ins0, suzanne, vec3(0, 0, -0.6), -1, vec3(1, 0, 0), vec3(0.2, 0.2, 0.2));
    instancer_instance_push(&inst, &ins0);

    struct texture tex2_mrao;
    texture_load_m(&tex2_mrao, "res/models/rustediron2_mrao.png", -0.4);
    material_mrao_map_set(suzanne->material, &tex2_mrao);

    mesh_gen_tangent(suzanne);

    renderer_mesh_register(&win.renderer, suzanne);

    struct texture hdri;
    texture_load_hdr(&hdri, "res/img/lush_dirt_path_4k.hdr");

    envmap_hdr_set(&win.renderer.environment, &hdri);


    camera_translate(&win.renderer.camera, vec3(-0.5, -0.5, -1));
    camera_rotate(&win.renderer.camera, vec3(1, 0, 0), 0.1);
    camera_rotate(&win.renderer.camera, vec3(0, 1, 0), -0.5);

    struct light l1;
    light_init(&l1, vec3(0, 0, 0), vec4(1, 1, 1, 1), LIGHT_POINT);
    renderer_light_register(&win.renderer, &l1);
    
    window_set_update(&win, update, NULL);

    window_main(&win);

    container_free(&c1);
    texture_free(&hdri);
    texture_free(&tex2_mrao);
    material_free(&material0);

    window_free(&win);
    return 0;
}





























#if 0

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

    int mx, my;
    mat4 cr;
    SDL_PumpEvents();
    SDL_GetMouseState(&mx, &my);

    glm_mat4_identity(cr);
    glm_translate(cr, vec3(0, 0, -1));

    glm_rotate(cr, (float)mx / 100, vec3(0, 1, 0));
    glm_rotate(cr, -(float)my / 100, vec3(1, 0, 0));

    glm_translate(cr, vec3(0, 0, 0.6));
    glm_mat4_copy(cr, renderer->camera.view);


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
    mesh_cull_from_normal(&mesh);
    mesh_cull_from_normal(&cube_mesh);

    mesh_gen_tangent(&mesh);
    //mesh_gen_tangent(&monkey_mesh);

    //monkey_mesh.has_shadow = 0;

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

    mesh_ivert_push_back(&mesh, ivert(
                vec3(0, 0, -0.6), 
                -1, vec3(1, 0, 0), 
                vec3(0.2, 0.2, 0.2), 
                vec2(0, 0)));
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

    camera_translate(&win.renderer.camera, vec3(-0.5, -0.5, -1));
    camera_rotate(&win.renderer.camera, vec3(1, 0, 0), 0.1);
    camera_rotate(&win.renderer.camera, vec3(0, 1, 0), -0.5);

    // -----------------------------------------------------------------------------
    // test for container

    struct ivert iv2 = {
        .trans = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        }
        ,0
    };
    glm_translate_make(iv2.trans, vec3(0, 0, -0.6));
    glm_rotate(iv2.trans, 2, vec3(1, 0, 0));
    glm_scale(iv2.trans, vec3(0.2, 0.2, 0.2));

    struct container c1;
    container_init(&c1);
    container_load_obj(&c1, "res/models/monkey.obj");
    //printf("%zu\n", darray_len(&c1.materials));

    renderer_mesh_register(&win.renderer, c1.meshes[0]);

    mesh_ivert_push_back(c1.meshes[0], iv2);

    //mesh_material_set(c1.meshes[0], c1.materials[0]);

    mesh_gen_tangent(c1.meshes[0]);


    // -----------------------------------------------------------------------------
    // Container load gltf test
#if 0
    struct ivert iv3 = {
        .trans = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        }
        ,0
    };
    glm_translate_make(iv2.trans, vec3(0, 0, -0.6));
    glm_rotate(iv2.trans, -1, vec3(1, 0, 0));
    glm_scale(iv2.trans, vec3(0.2, 0.2, 0.2));
    
    struct container c2;
    container_init(&c2);
    container_load_gltf(&c2, "res/models/cube.gltf");

    //printf("%zu\n", darray_len(&c2.meshes));

    mesh_ivert_push_back(c2.meshes[0], iv3);
    //printf("%zu\n", glbuf_size(&c2.meshes[0]->vbo));
    renderer_mesh_register(&win.renderer, c2.meshes[0]);
#endif

    

    // texture
    struct material material0, material1;
    material_init(&material0);
    material_init(&material1);

    struct texture tex1, tex1_normal, tex1_mrao;
    texture_load(&tex1, "res/img/wood_table/wood_table_001_diff_4k.jpg");
    texture_load(&tex1_normal, "res/img/wood_table/wood_table_001_nor_gl_4k.jpg");
    texture_load(&tex1_mrao, "res/img/wood_table/wood_table_001_mra_4k.jpg");

    struct texture tex2, tex2_normal, tex2_mrao, tex2_emission;
    texture_load_m(&tex2, "res/img/rustediron2_basecolor_linear.png", - 0.4);
    texture_load_m(&tex2_normal, "res/img/rustediron2_normal.png", -0.4);
    texture_load_m(&tex2_mrao, "res/img/rustediron2_mrao.png", -0.4);
    //texture_load(&tex2_emission, "res/img/monkey_emission.png");
    
    material_albedo_map_set(&material0, &tex1);
    material_normal_map_set(&material0, &tex1_normal, 0.01);

    material_albedo_map_set(&material1, &tex2);
    material_mrao_map_set(&material1, &tex2_mrao);
    material_normal_map_set(&material1, &tex2_normal, 1.0);
    //material_emission_map_set(&material1, &tex2_emission);
    glm_vec4_copy(vec4(0, 1, 1, 0), material1.mrao);
    glm_vec4_copy(vec4(1, 1, 1, 0.0), material1.emission);

    mesh_material_set(c1.meshes[0], &material1);
    //mesh_material_set(c2.meshes[0], &material0);


    //mesh_texture_push(&mesh, tex1);
#if 0
    mesh_texture_albedo_set(&mesh, &tex1);
    mesh_texture_normal_set(&mesh, &tex1_normal);
    mesh_texture_albedo_set(&monkey_mesh, &tex1);
#endif
    mesh_material_set(&mesh, &material0);
    //mesh_material_set(&monkey_mesh, &material1);
    //mesh_texture_normal_set(&monkey_mesh, &tex1_normal);

    light_init(&light0, vec4(0, 1, -0.5, 1), vec4(1, 1, 1, 1), LIGHT_POINT);
    light_init(&light1, vec4(-0.4, 0, 0, 1), vec4(1, 1, 1, 0.5), LIGHT_POINT);
    //light1.shadow_len = 1;
    //light0.casts_shadow = 0;

    renderer_mesh_register(&win.renderer, &mesh);
    //renderer_mesh_register(&win.renderer, &monkey_mesh);
    renderer_mesh_register(&win.renderer, &cube_mesh);
    renderer_light_register(&win.renderer, &light0);
    renderer_light_register(&win.renderer, &light1);

    struct texture hdri;
    texture_load_hdr(&hdri, "res/img/christmas_photo_studio_05_4k.hdr");
    
    envmap_hdr_set(&win.renderer.environment, &hdri);
    
    //glm_vec4_copy(vec4(1, 1, 1, 1), material1.emission);

    window_set_update(&win, update, NULL);

    window_main(&win);

    window_free(&win);
}

#endif
