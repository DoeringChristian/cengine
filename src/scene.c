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

int scene_draw(struct scene *dst, struct camera *camera){
    if(dst->shader == NULL)
        return 1;
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        mesh_draw(dst->meshes[i], camera);
    }
    return 0;
}
int scene_lvert_push(struct scene *dst, struct lvert src){
    return darray_push_back(&dst->lights, src);
}
