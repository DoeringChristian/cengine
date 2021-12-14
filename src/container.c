#include "container.h"

int container_init(struct container *dst){
    darray_init(&dst->meshes, 0);
    darray_init(&dst->cmeshes, 0);
    darray_init(&dst->lights, 0);
    darray_init(&dst->materials, 0);
    darray_init(&dst->textures, 0);
    darray_init(&dst->sub_containers, 0);
    return 0;
}

void container_free(struct container *dst){

    for (int i = 0; i < darray_len(&dst->meshes); i++) {
        mesh_free(dst->meshes[i]);
        free(dst->meshes[i]);
    }
    for (int i = 0; i < darray_len(&dst->cmeshes); i++) {
        cmesh_free(dst->cmeshes[i]);
        free(dst->meshes[i]);
    }
    for (int i = 0; i < darray_len(&dst->lights); i++) {
        //light_free(dst->lights[i]);
        free(dst->meshes[i]);
    }
    for (int i = 0; i < darray_len(&dst->materials); i++) {
        material_free(dst->materials[i]);
        free(dst->meshes[i]);
    }
    for (int i = 0; i < darray_len(&dst->textures); i++) {
        texture_free(dst->textures[i]);
        free(dst->meshes[i]);
    }
    for (int i = 0; i < darray_len(&dst->sub_containers); i++) {
        container_free(dst->sub_containers[i]);
        free(dst->sub_containers[i]);
    }

    darray_free(&dst->meshes);
    darray_free(&dst->cmeshes);
    darray_free(&dst->lights);
    darray_free(&dst->materials);
    darray_free(&dst->textures);
    darray_free(&dst->sub_containers);
}
struct mesh *container_mesh_search(struct container *src, const char *name){
    for(size_t i = 0;i < darray_len(&src->meshes);i++){
        if(resource_handle_name_comp(&src->meshes[i]->handle, name))
            return src->meshes[i];
    }
    return NULL;
}
struct mesh *container_mesh_search_path(struct container *src, const char *path){
    for(size_t i = 0;i < darray_len(&src->meshes);i++){
        if(resource_handle_path_comp(&src->meshes[i]->handle, path))
            return src->meshes[i];
    }
    return NULL;
}

struct material *container_material_search(struct container *src, const char *name){
    for(size_t i = 0;i < darray_len(&src->materials);i++){
        if(resource_handle_name_comp(&src->materials[i]->handle, name))
            return src->materials[i];
    }
    return NULL;
}
struct material *container_material_search_path(struct container *src, const char *path){
    for(size_t i = 0;i < darray_len(&src->materials);i++){
        if(resource_handle_path_comp(&src->materials[i]->handle, path))
            return src->materials[i];
    }
    return NULL;
}
