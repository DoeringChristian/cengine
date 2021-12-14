#include "resource_handle.h"

int resource_handle_init(struct resource_handle *dst, const char *name, const char *path){
    dst->name = NULL;
    dst->path = NULL;
    if(name != NULL){
        size_t n = strlen(name) + 1;
        dst->name = malloc(n);
        memcpy(dst->name, name, n);
    }
    if(path != NULL){
        size_t n = strlen(path) + 1;
        dst->path = malloc(n);
        memcpy(dst->path, path, n);
    }
    return 0;
}
void resource_handle_free(struct resource_handle *dst){
    if(dst->name != NULL){
        free(dst->name);
        dst->name = NULL;
    }
    if(dst->path != NULL){
        free(dst->path);
        dst->path = NULL;
    }
}

int resource_handle_name_set(struct resource_handle *dst, const char *name){
    if(dst->name != NULL){
        free(dst->name);
        dst->name = NULL;
    }
    if(name != NULL){
        size_t n = strlen(name) + 1;
        dst->name = malloc(n);
        memcpy(dst->name, name, n);
    }
    return 0;
}
int resource_handle_path_set(struct resource_handle *dst, const char *path){
    if(dst->name != NULL){
        free(dst->name);
        dst->name = NULL;
    }
    if(path != NULL){
        size_t n = strlen(path) + 1;
        dst->path = malloc(n);
        memcpy(dst->path, path, n);
    }
    return 0;
}

int resource_handle_name_comp(struct resource_handle *src, const char *name){
    size_t name_size = strlen(name);
    if(src->name != NULL && (name_size = strlen(name)) == strlen(src->name) && memcmp(name, src->name, name_size) == 0)
        return 1;
    return 0;
}
int resource_handle_path_comp(struct resource_handle *src, const char *path){
    size_t path_size = strlen(path);
    if(src->path != NULL && (path_size = strlen(path)) == strlen(src->name) && memcmp(path, src->path, path_size) == 0)
        return 1;
    return 0;
}
