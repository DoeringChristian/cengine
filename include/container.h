#ifndef CONTAINER_H
#define CONTAINER_H

#include "darray.h"
#include "mesh.h"
#include "cmesh.h"
#include "texture.h"
#include "material.h"

struct container{
    darray(struct mesh*) meshes;
    darray(struct cmesh*) cmeshes;
    darray(struct texture*) textures;
    darray(struct material*) materials;
    darray(struct light*) lights;
};

int container_init(struct container *dst);
int container_load(struct container *dst, const char *path);

int container_unload(struct container *dst);
void container_free(struct container *dst);

int container_merge(struct container *dst, struct container *src);

struct mesh *container_mesh_search(struct container *src, const char *name);
struct mesh *container_mesh_search_path(struct container *src, const char *path);

struct material *container_material_search(struct container *src, const char *name);
struct material *container_material_search_path(struct container *src, const char *path);

#endif //CONTAINER_H
