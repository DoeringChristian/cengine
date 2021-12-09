#ifndef GL_UTIL_H
#define GL_UTIL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <stdio.h>
#include <signal.h>
#include "cglm/cglm.h"

#define GLCall(_x){\
    gl_err_clear();\
    _x;\
    gl_err_print(__FILE__, __func__, __LINE__);\
}

#ifndef MIN
#define MIN(_x, _y) ((_x) < (_y) ? (_x) : (_y))
#endif

#ifndef MAX
#define MAX(_x, _y) ((_x) > (_y) ? (_x) : (_y))
#endif

#define vao_attr_push(_idx, _type, _normalized, _struct, _element){\
    _struct _tmp;\
    GLCall(glEnableVertexAttribArray(_idx));\
    GLCall(glVertexAttribPointer(_idx, sizeof(_tmp._element)/attr_size(_type), _type, _normalized, sizeof(_tmp), (void *)offsetof(_struct, _element)));\
}

#define vao_attr_push_inc(_idx, _type, _normalized, _struct, _element){\
    vao_attr_push(_idx, _type, _normalized, _struct, _element);\
    (_idx)++;\
}

#define vao_attr_push_mat4f_inc(_idx, _type, _normalized, _struct, _element){\
    _struct _tmp;\
    GLCall(glEnableVertexAttribArray(_idx));\
    GLCall(glVertexAttribPointer(_idx, (sizeof(_tmp._element)/4)/attr_size(_type), _type, _normalized, sizeof(_tmp), (0 * attr_size(_type)) + (void *)offsetof(_struct, _element)));\
    (_idx)++;\
\
    GLCall(glEnableVertexAttribArray(_idx));\
    GLCall(glVertexAttribPointer(_idx, (sizeof(_tmp._element)/4)/attr_size(_type), _type, _normalized, sizeof(_tmp), (1 * attr_size(_type)) + (void *)offsetof(_struct, _element)));\
    (_idx)++;\
\
    GLCall(glEnableVertexAttribArray(_idx));\
    GLCall(glVertexAttribPointer(_idx, (sizeof(_tmp._element)/4)/attr_size(_type), _type, _normalized, sizeof(_tmp), (2 * attr_size(_type)) + (void *)offsetof(_struct, _element)));\
    (_idx)++;\
\
    GLCall(glEnableVertexAttribArray(_idx));\
    GLCall(glVertexAttribPointer(_idx, (sizeof(_tmp._element)/4)/attr_size(_type), _type, _normalized, sizeof(_tmp), (3 * attr_size(_type)) + (void *)offsetof(_struct, _element)));\
    (_idx)++;\
}

#define vao_attr_push_mat4f_div_inc(_idx, _type, _normalized, _struct, _element){\
    attr_push_mat4f_div_inc(&(_idx), _normalized, sizeof(_struct), (void *)offsetof(_struct, _element));\
}

// defining vec initialisations
#define vec2(_x, _y) ((float [2]){_x, _y})
#define vec3(_x, _y, _z) ((float [3]){_x, _y, _z})
#define vec4(_x1, _x2, _x3, _x4) ((float [4]){_x1, _x2, _x3, _x4})
#define versor(_x, _y, _z, _w) ((float [4]){_x, _y, _z, _w})

void gl_err_clear();
void gl_err_print(const char *file, const char *func, int line);

size_t attr_size(GLenum type);

int attr_push_mat4f_div_inc(int *idx, GLboolean normalized, GLsizei stride, const void *offset);

#endif //GL_UTIL_H
