#include "renderer.h"

int renderer_init(struct renderer *dst, int w, int h){
    //darray_init(&dst->instances, 10);
    dst->w = w;
    dst->h = h;
    
    shader_load(&dst->shader, "shaders/vert.glsl", "shaders/frag.glsl");

    layer_init(&dst->l1, w, h, "shaders/layer/vert.glsl", "shaders/layer/frag.glsl");

    gbuf_init(&dst->gbuf, w, h);

    dst->scene = NULL;
    return 0;
}
void renderer_free(struct renderer *dst){
    layer_free(&dst->l1);
    shader_free(&dst->shader);
    layer_free(&dst->l1);
    gbuf_free(&dst->gbuf);
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

    struct shader light_shader;
    shader_load(&light_shader, "shaders/lighting/vert_ssp.glsl", "shaders/lighting/frag_lighting.glsl");
    struct lvert light = {
        .pos = {1, 1, 1, 1},
        .color = {1, 0, 0, 1},
    };

    struct shader layer_shader;
    shader_load(&layer_shader, "shaders/layer/vert.glsl", "shaders/layer/frag.glsl");

    gbuf_bind(&src->gbuf);
    scene_draw(src->scene);
    gbuf_unbind(&src->gbuf);

    gbuf_draw(&src->gbuf, &light_shader, light);


    //layer_bind(&src->l1);
    //layer_unbind(&src->l1);

    layer_draw(&src->l1, &layer_shader);

    glBindVertexArray(0);
    glUseProgram(0);
    return 0;
}


int renderer_scene_set(struct renderer *dst, struct scene *src){
    dst->scene = src;
    src->shader = &dst->shader;
    return 0;
}
