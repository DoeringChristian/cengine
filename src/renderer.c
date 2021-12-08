#include "renderer.h"

int renderer_init(struct renderer *dst, int w, int h){
    //darray_init(&dst->instances, 10);
    dst->w = w;
    dst->h = h;
    

    shader_load(&dst->shader, "shaders/vert.glsl", "shaders/frag.glsl");

    gbuf_init_shader(&dst->gbuf, w, h, "shaders/lighting/vert_ssp.glsl", "shaders/lighting/frag_lighting.glsl");

    shader_load(&dst->shader_forward, "shaders/layer/frag.glsl", "shaders/layer/frag.glsl");

#if 1
    layer_init_shader(&dst->light, w, h, "shaders/lighting/vert_ssp.glsl", "shaders/layer/frag.glsl");
    layer_init_shader(&dst->light_sum, w, h, "shaders/lighting/vert_ssp.glsl", "shaders/layer/frag.glsl");
    layer_init_shader(&dst->light_tmp, w, h, "shaders/lighting/vert_ssp.glsl", "shaders/layer/frag.glsl");
#endif

#if 0
    layer_init(&dst->light, w, h);
    layer_init(&dst->light_sum, w, h);
    layer_init(&dst->light_tmp, w, h);
#endif

    shader_load(&dst->shader_sum, "shaders/lighting/vert_ssp.glsl", "shaders/lighting/frag_add.glsl");

    camera_init(&dst->camera, w, h, 1);

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
    shader_free(&dst->shader_sum);
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

    // light test:
    float light_pos[4] = {
        1, 1, 1, 1,
    };
    float light_color[4] = {
        1, 1, 1, 1,
    };

    struct lvert light0 = {
        .pos = {1, 1, 1, 1},
        .color = {1, 0, 0, 1},
    };
    struct lvert light1 = {
        .pos = {-1, -1, 1, 1},
        .color = {0, 1, 0, 1},
    };


    // render the scene to the gbuf

    gbuf_bind(&src->gbuf);
    scene_draw(src->scene, &src->camera);
    gbuf_unbind(&src->gbuf);

#if 1
    // clear the summ of lightess maps
    layer_bind(&src->light_sum);
    layer_unbind(&src->light_sum);

    for(size_t i = 0;i < darray_len(&src->scene->lights);i++){
        // calculate view projection of light
        struct lvert light_tmp = src->scene->lights[i];
        glm_mat4_mulv(src->camera.mat, src->scene->lights[i].pos, light_tmp.pos);
        //vec4_multiply_mat4(light_tmp.pos, src->scene->lights[i].pos, src->camera.mat);

        // render the albedo of the gbuf to the light layer
        layer_bind(&src->light);
        gbuf_draw(&src->gbuf, light_tmp);
        layer_unbind(&src->light);

        // sum the lightness map with all previous. and store it into the tmp layer.
        
        layer_blend(&src->light_tmp, &src->light_sum, &src->light, &src->shader_sum);

        // copy it to the light sum layer
        layer_bind(&src->light_sum);
        layer_draw(&src->light_tmp);
        layer_unbind(&src->light_sum);

    }
#endif

# if 0
    struct layer tmp;
    layer_init_shader(&tmp, src->w, src->h, "shaders/layer/vert.glsl", "shaders/layer/frag.glsl");

    layer_bind(&src->light);
    gbuf_draw(&src->gbuf, light0);
    layer_unbind(&src->light);

    layer_blend(&tmp, &src->light_sum, &src->light, &src->shader_sum);

    layer_bind(&src->light_sum);
    layer_draw(&tmp);
    layer_unbind(&src->light_sum);


#if 1

    layer_bind(&src->light);
    gbuf_draw(&src->gbuf, light1);
    layer_unbind(&src->light);

    layer_blend(&tmp, &src->light_sum, &src->light, &src->shader_sum);

    layer_bind(&src->light_sum);
    layer_draw(&tmp);
    layer_unbind(&src->light_sum);
#endif
#endif

    // draw the lightness map
    //layer_draw_shader(&src->light_sum, &src->shader_forward);
    layer_draw(&src->light_sum);

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
