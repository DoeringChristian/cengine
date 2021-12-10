#ifndef LOADER_OBJ_H
#define LOADER_OBJ_H

#include "mesh.h"
#include <stdio.h>

int mesh_load_obj(struct mesh *dst, const char *path);

#endif //LOADER_OBJ_H
