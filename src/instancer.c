#include "instancer.h"

int instance_init(struct instance *dst, struct mesh *mesh, vec3 pos, float angle, vec3 axis, vec3 scale){
    dst->ivert = ivert(pos, angle, axis, scale, vec2(0, 0));
    dst->mesh = mesh;
    return 0;
}

int instancer_init(struct instancer *dst){
    darray_init(&dst->instances, 0);

    return 0;
}
void instancer_free(struct instancer *dst){
    darray_free(&dst->instances);
}

int instancer_update(struct instancer *dst){
    for(size_t i = 0;i < darray_len(&dst->instances);i++){
        mesh_iverts_clear(dst->instances[i]->mesh);
    }
    for(size_t i = 0;i < darray_len(&dst->instances);i++){
        mesh_ivert_push_back(dst->instances[i]->mesh, dst->instances[i]->ivert);
    }
    return 0;
}
int instancer_instance_push(struct instancer *dst, struct instance *src){
    darray_push_back(&dst->instances, src);
    return 0;
}
int instancer_instance_pop(struct instancer *dst, struct instance *target){
    for(size_t i = 0;i < darray_len(&dst->instances);){
        if(dst->instances[i] == target)
            darray_pop(&dst->instances, i);
        else
            i++;
    }
    return 0;
}
