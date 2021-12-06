#ifndef INSTANCE_H
#define INSTANCE_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include "darray.h"
#include "mesh.h"

struct instance{
    struct mesh *mesh;
    struct ivert ivert;
};

int instance_init(struct instance *dst, struct mesh *src);
void instance_free(struct instance *dst);

int instance_push(struct instance *dst);

#endif //INSTANCE_H
