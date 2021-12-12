#ifndef GLMESH_H
#define GLMESH_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "glbuf.h"

struct glmesh{
    struct glbuf vbo;
    struct glbuf ibo;
    GLuint gl_vao;
};

int glmesh_init_quad(struct glmesh *dst);
int glmesh_init_cube(struct glmesh *dst);

#endif //GLMESH_H
