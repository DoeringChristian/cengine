#include "scene.h"

int scene_init(struct scene *dst){
    darray_init(&dst->meshes, 10);

    dst->shader = NULL;

    texture_init_f32(&dst->lights, 0, 2, NULL);

    return 0;
}
void scene_free(struct scene *dst){
    darray_free(&dst->meshes);
    texture_free(&dst->lights);
}

int scene_mesh_push_back(struct scene *dst, struct mesh *src){
    darray_push_back(&dst->meshes, src);
    src->lights = &dst->lights;
    return 0;
}

int scene_draw(struct scene *dst){
    if(dst->shader == NULL)
        return 1;
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        mesh_draw(dst->meshes[i]);
    }
    return 0;
}
