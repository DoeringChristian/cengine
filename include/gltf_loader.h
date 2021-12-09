#ifndef GLTF_LOADER_H
#define GLTF_LOADER_H

#include "mesh.h"
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

int mesh_load_gltf(struct mesh *dst, const char *path, int idx);

float readf32(void *src);

#endif //GLTF_LOADER_H
