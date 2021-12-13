#ifndef LOADER_OBJ_H
#define LOADER_OBJ_H

#include "mesh.h"
#include "container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: eneble loading of one vertex for multiple faces.
int mesh_load_obj(struct mesh *dst, const char *path);

int container_load_obj(struct container *dst, const char *path);
int container_load_mtl(struct container *dst, const char *path);

#endif //LOADER_OBJ_H
