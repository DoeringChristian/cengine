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
    darray(struct container*) sub_containers;
};

int container_init(struct container *dst);
int container_load(struct container *dst, const char *path);

int container_unload(struct container *dst);
void container_free(struct container *dst);

#endif //CONTAINER_H
