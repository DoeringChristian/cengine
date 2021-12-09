#include "scene.h"

int scene_init(struct scene *dst){
    darray_init(&dst->meshes, 10);
    darray_init(&dst->lights, 10);

    dst->shader = NULL;

    return 0;
}
void scene_free(struct scene *dst){
    darray_free(&dst->meshes);
    darray_free(&dst->lights);
}

int scene_mesh_push_back(struct scene *dst, struct mesh *src){
    darray_push_back(&dst->meshes, src);
    return 0;
}

int scene_draw(struct scene *dst, struct cvert *camera, struct shader *shader){
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        mesh_draw(dst->meshes[i], camera, shader);
    }
    return 0;
}
int scene_draw_shadow_depth(struct scene *dst, struct cvert *camera, struct shader *shader, struct light *light){
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        mesh_draw_depth(dst->meshes[i], camera, shader, light);
    }
}
int scene_light_push(struct scene *dst, struct light src){
    return darray_push_back(&dst->lights, src);
}
