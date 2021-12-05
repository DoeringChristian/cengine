#include "attr_arr.h"

size_t attr_size(GLenum type){
    switch(type){
    case GL_BYTE:
        return 1;
    case GL_UNSIGNED_BYTE:
        return 1;
    case GL_SHORT:
        return 2;
    case GL_UNSIGNED_SHORT:
        return 2;
    case GL_INT:
        return 4;
    case GL_UNSIGNED_INT:
        return 4;
    case GL_FIXED:
        return 4;
    case GL_HALF_FLOAT:
        return 2;
    case GL_FLOAT:
        return 4;
    case GL_DOUBLE:
        return 8;
    }
    return 0;
}

struct attr_arr attr_arr(GLsizei stride){
    return (struct attr_arr){.stride = stride, .idx = 0, .offset = 0};
}

int attr_arr_push(struct attr_arr *dst, GLsizei num, GLenum type, GLint normalized){
    GLCall(glEnableVertexAttribArray(dst->idx));
    GLCall(glVertexAttribPointer(dst->idx, num, type, normalized, dst->stride, (void *)dst->offset));
    dst->idx++;
    dst->offset += num * attr_size(type);

    return 0;
}
