#include "renderer.h"

#define SHADOW_SIZE 1024

int renderer_init(struct renderer *dst, int w, int h){
    //darray_init(&dst->instances, 10);
    dst->w = w;
    dst->h = h;
    

    shader_load(&dst->shader, "shaders/vert.glsl", "shaders/frag.glsl");

    shader_load(&dst->shader_forward, "shaders/layer/frag.glsl", "shaders/layer/frag.glsl");

    shader_load(&dst->shader_mul, "shaders/lighting/vert_ssp.glsl", "shaders/lighting/frag_mul.glsl");

    shader_load(&dst->shader_add, "shaders/lighting/vert_ssp.glsl", "shaders/lighting/frag_add.glsl");

    shader_load_vgf(&dst->shader_shadow, "shaders/lighting/vert_shadow.glsl", "shaders/lighting/geo_shadow.glsl", "shaders/lighting/frag_shadow.glsl");

    gbuf_init_shader(&dst->gbuf, w, h, "shaders/lighting/vert_ssp.glsl", "shaders/lighting/frag_lighting.glsl");

#if 1
    layer_init_shader(&dst->light, w, h, "shaders/lighting/vert_ssp.glsl", "shaders/layer/frag.glsl");
    layer_init_shader(&dst->light_sum, w, h, "shaders/lighting/vert_ssp.glsl", "shaders/layer/frag.glsl");
    layer_init_shader(&dst->light_tmp, w, h, "shaders/lighting/vert_ssp.glsl", "shaders/layer/frag.glsl");

    layer_init_cube(&dst->shadow_cube, SHADOW_SIZE);
    cubelayer_init(&dst->cl_shadow, SHADOW_SIZE, SHADOW_SIZE);
#endif

#if 0
    layer_init(&dst->light, w, h);
    layer_init(&dst->light_sum, w, h);
    layer_init(&dst->light_tmp, w, h);
#endif

    cvert_init(&dst->camera, w, h, 1);

    dst->scene = NULL;
    return 0;
}
void renderer_free(struct renderer *dst){
    shader_free(&dst->shader);
    gbuf_free(&dst->gbuf);
    layer_free(&dst->light);
    layer_free(&dst->light_sum);
    layer_free(&dst->light_tmp);
    shader_free(&dst->shader_forward);
    shader_free(&dst->shader_add);
    shader_free(&dst->shader);
}

int renderer_push(struct renderer *dst){
    // - clear all iverts from meshes
    // - push all iverts from instances
    // - update all meshes
    
    return 0;
}

int renderer_render(struct renderer *src){
    if(src->scene == NULL)
        return 1;
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // render the scene to the gbuf

    gbuf_bind(&src->gbuf);
    scene_draw(src->scene, &src->camera, &src->shader);
    gbuf_unbind(&src->gbuf);

    // clear the summ of lightess maps
    layer_bind(&src->light_sum);
    layer_unbind(&src->light_sum);

    for(size_t i = 0;i < darray_len(&src->scene->lights);i++){
        // render shadow cube map
        
        struct cvert cm_cameras[6];
        cvert_init(&cm_cameras[0], 1, 1, M_PI/2);
        glm_perspective(M_PI/2, 1, 0.1, 100, cm_cameras[0].proj);

        glm_look(src->scene->lights[i].pos, (float []){1, 0, 0}, (float []){0, -1, 0}, cm_cameras[0].view);
        glm_look(src->scene->lights[i].pos, (float []){-1, 0, 0}, (float []){0, -1, 0}, cm_cameras[1].view);
        glm_look(src->scene->lights[i].pos, (float []){0, 1, 0}, (float []){0, 0, 1}, cm_cameras[2].view);
        glm_look(src->scene->lights[i].pos, (float []){0, -1, 0}, (float []){0, 0, 1}, cm_cameras[3].view);
        glm_look(src->scene->lights[i].pos, (float []){0, 0, 1}, (float []){0, -1, 0}, cm_cameras[4].view);
        glm_look(src->scene->lights[i].pos, (float []){0, 0, -1}, (float []){0, -1, 0}, cm_cameras[5].view);
        
#if 0
        for(size_t k = 0;k < 6;k++){
            for(size_t j = 0;j < 16;j++){
                printf("%f ", ((float *)cm_cameras[k].view)[j]);
                if((j + 1) % 4 == 0)
                    printf("\n");
            }
            printf("\n");
        }
        printf("\n");
#endif

        // calculate view projection of light
        struct lvert light_tmp = src->scene->lights[i];
        glm_mat4_mulv(src->camera.proj, src->scene->lights[i].pos, light_tmp.pos);
#if 1
        //layer_bind(&src->shadow_cube);
        cubelayer_bind(&src->cl_shadow);
        scene_draw_shadow_cm(src->scene, cm_cameras, &src->shader_shadow, &light_tmp);
        cubelayer_unbind(&src->cl_shadow);
        //layer_unbind(&src->shadow_cube);
#endif


        
        //vec4_multiply_mat4(light_tmp.pos, src->scene->lights[i].pos, src->camera.mat);

        // render the light of the gbuf to the light layer
        layer_bind(&src->light);
        gbuf_draw(&src->gbuf, light_tmp, &src->cl_shadow.texture, &src->camera);
        layer_unbind(&src->light);

        // sum the lightness map with all previous. and store it into the tmp layer.
        
        layer_blend(&src->light_tmp, &src->light_sum, &src->light, &src->shader_add);

        // copy it to the light sum layer
        layer_bind(&src->light_sum);
        layer_draw(&src->light_tmp);
        layer_unbind(&src->light_sum);

    }

    // reset viewport
    glViewport(0, 0, src->w, src->h);

    // draw the lightness map
    //layer_draw_shader(&src->light_sum, &src->shader_forward);
    
    layer_draw(&src->light_sum);

#if 0
    struct shader tmp;
    shader_load(&tmp, "shaders/layer/vert.glsl", "shaders/layer/frag_cube.glsl");
    //cubelayer_draw(&src->cl_shadow, &tmp);
    shader_free(&tmp);
#endif

#if 0
    struct shader tmp;
    shader_load(&tmp, "shaders/layer/vert.glsl", "shaders/layer/frag_cube.glsl");
    layer_draw_shader(&src->shadow_cube, &tmp);
    shader_free(&tmp);
#endif

    //layer_clear(&src->light_sum);

    //layer_bind(&src->l1);
    //layer_unbind(&src->l1);

    //layer_draw_shader(&src->l1, &layer_shader);

    glBindVertexArray(0);
    glUseProgram(0);
    return 0;
}


int renderer_scene_set(struct renderer *dst, struct scene *src){
    dst->scene = src;
    src->shader = &dst->shader;
    return 0;
}
