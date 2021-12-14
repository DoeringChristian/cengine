#ifndef LOADER_GLTF_H
#define LOADER_GLTF_H

#include "mesh.h"
#include "container.h"
#include "cgltf.h"

int mesh_load_gltf(struct mesh *dst, cgltf_mesh *src);

int container_load_gltf(struct container *dst, const char *path);

#endif //LOADER_GLTF_H
