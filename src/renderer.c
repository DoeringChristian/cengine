#include "renderer.h"

int renderer_init(struct renderer *dst){
    darray_init(&dst->meshes, 10);
    darray_init(&dst->instances, 10);
    
    shader_load(&dst->shader, "shaders/vert.glsl", "shaders/frag.glsl");

}
void renderer_free(struct renderer *dst){
#if 0
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        mesh_free(dst->meshes[i]);
        free(dst->meshes[i]);
    }
#endif
    darray_free(&dst->meshes);
    shader_free(&dst->shader);
}

int renderer_push(struct renderer *dst){
    // - clear all iverts from meshes
    // - push all iverts from instances
    // - update all meshes
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        mesh_iverts_clear(dst->meshes[i]);
    }
    for(size_t i = 0;i < darray_len(&dst->instances);i++){
        instance_push(dst->instances[i]);
    }
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        mesh_push(dst->meshes[i]);
    }
    return 0;
}

int renderer_render(struct renderer *src){
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(src->shader.program);

    for(size_t i = 0;i < darray_len(&src->meshes);i++){
        mesh_draw(src->meshes[i]);
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

int renderer_mesh_push(struct renderer *dst, struct mesh *src){
    return darray_push_back(&dst->meshes, src);
}
int renderer_instance_push(struct renderer *dst, struct instance *src){
    return darray_push_back(&dst->instances, src);
}
