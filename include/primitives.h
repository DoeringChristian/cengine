#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "mesh.h"

int mesh_init_quad(struct mesh *dst, struct shader *shader);
int mesh_init_cube(struct mesh *dst, struct shader *shader);

#endif //PRIMITIVES_H
