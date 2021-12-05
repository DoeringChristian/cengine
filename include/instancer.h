#ifndef INSTANCER_H
#define INSTANCER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include "darray.h"
#include "mathc.h"
#include "attr_arr.h"

struct instance{
    float trans[16];
    float tex_idx_offset;
};

struct instancer{
    darray(struct instance) instances;

    struct attr_arr *attr_arr;

    GLuint gl_vbo;
    size_t gl_vbo_size;
};

int instancer_init(struct instancer *dst, struct attr_arr *attr_arr);
void instancer_free(struct instancer *dst);

int instancer_resize(struct instancer *dst);
int instancer_update(struct instancer *dst);

int instancer_push(struct instancer *dst, struct instance src);

int instancer_attr_push(struct instancer *dst, GLsizei num, GLenum type, GLint normalized);

#endif //INSTANCER_H
