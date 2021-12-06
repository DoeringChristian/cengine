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

void gl_err_clear();
void gl_err_print(const char *file, const char *func, int line);

#endif //GL_UTIL_H
