#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "mesh.h"
#include "cmesh.h"

int mesh_init_quad(struct mesh *dst);
int mesh_init_cube(struct mesh *dst);

int cmesh_init_quad(struct cmesh *dst);
int cmesh_init_cube(struct cmesh *dst);

#endif //PRIMITIVES_H
