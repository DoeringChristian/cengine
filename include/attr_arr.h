#ifndef ATTR_ARR_H
#define ATTR_ARR_H

#include <stdint.h>
#include <stddef.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"

struct attr_arr{
    size_t offset;
    GLsizei stride;
    GLsizei idx;
};

struct attr_arr attr_arr(GLsizei stride);
int attr_arr_push(struct attr_arr *dst, GLsizei num, GLenum type, GLint normalized);
size_t attr_size(GLenum type);

#endif //ATTR_ARR_H
