#include "renderer.h"

int renderer_init(struct renderer *dst, int w, int h){
    //darray_init(&dst->instances, 10);
    dst->w = w;
    dst->h = h;
    
    shader_load(&dst->shader, "shaders/vert.glsl", "shaders/frag.glsl");

    layer_init(&dst->l1, w, h, "shaders/layer/vert.glsl", "shaders/layer/frag.glsl");

    gbuf_init(&dst->gbuf, w, h, "shaders/gbuf/vert.glsl", "shaders/gbuf/frag.glsl");

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

    gbuf_bind(&src->gbuf);
    scene_draw(src->scene);
    gbuf_unbind(&src->gbuf);

    layer_bind(&src->l1);
    gbuf_draw_debug(&src->gbuf);
    layer_unbind(&src->l1);
    

    layer_draw(&src->l1);

    glBindVertexArray(0);
    glUseProgram(0);
    return 0;
}


int renderer_scene_set(struct renderer *dst, struct scene *src){
    dst->scene = src;
    src->shader = &dst->shader;
    return 0;
}
