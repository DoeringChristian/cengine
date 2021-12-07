#include "renderer.h"

int renderer_init(struct renderer *dst){
    //darray_init(&dst->instances, 10);
    
    shader_load(&dst->shader, "shaders/vert.glsl", "shaders/frag.glsl");

    dst->scene = NULL;
    return 0;
}
void renderer_free(struct renderer *dst){
#if 0
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        mesh_free(dst->meshes[i]);
        free(dst->meshes[i]);
    }
#endif
    shader_free(&dst->shader);
}

int renderer_push(struct renderer *dst){
    // - clear all iverts from meshes
    // - push all iverts from instances
    // - update all meshes
    
#if 0
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        mesh_iverts_clear(dst->meshes[i]);
    }
    for(size_t i = 0;i < darray_len(&dst->instances);i++){
        instance_push(dst->instances[i]);
    }
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        mesh_push(dst->meshes[i]);
    }
#endif
    return 0;
}

int renderer_render(struct renderer *src){
    if(src->scene == NULL)
        return 1;
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(src->shader.program);

    scene_draw(src->scene);

    glBindVertexArray(0);
    glUseProgram(0);
    return 0;
}


int renderer_scene_set(struct renderer *dst, struct scene *src){
    dst->scene = src;
    src->shader = &dst->shader;
    return 0;
}
