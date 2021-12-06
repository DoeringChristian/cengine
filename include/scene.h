#ifndef SCENEH_H
#define SCENEH_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "mesh.h"
#include "darray.h"

struct scene{
    darray(struct mesh *) meshes;

    struct shader *shader;
};

#endif //SCENEH_H
