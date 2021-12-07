#ifndef GL_UTIL_H
#define GL_UTIL_H

#include <GL/gl.h>
#include <stdio.h>
#include <signal.h>

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

void gl_err_clear();
void gl_err_print(const char *file, const char *func, int line);

size_t attr_size(GLenum type);

#endif //GL_UTIL_H
