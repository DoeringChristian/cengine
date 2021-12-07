#include "renderer.h"

int renderer_init(struct renderer *dst, int w, int h){
    //darray_init(&dst->instances, 10);
    dst->w = w;
    dst->h = h;
    
    shader_load(&dst->shader, "shaders/vert.glsl", "shaders/frag.glsl");

    target_init(&dst->t1, w, h, "shaders/gbuf/vert.glsl", "shaders/gbuf/frag.glsl");

    dst->scene = NULL;
    return 0;
}
void renderer_free(struct renderer *dst){
    target_free(&dst->t1);
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

    target_bind(&src->t1);

    scene_draw(src->scene);

    target_unbind(&src->t1);
    
    target_draw(&src->t1);

    glBindVertexArray(0);
    glUseProgram(0);
    return 0;
}


int renderer_scene_set(struct renderer *dst, struct scene *src){
    dst->scene = src;
    src->shader = &dst->shader;
    return 0;
}
