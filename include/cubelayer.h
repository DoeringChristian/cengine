#ifndef CUBELAYER_H
#define CUBELAYER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include "glbuf.h"
#include "texture.h"

struct cubelayer{
    struct glbuf vbo;
    struct glbuf ibo;

    int w, h;

    GLuint gl_fbo;
    GLuint gl_vao;
    GLuint gl_rbo;

    GLuint texture;
};

int cubelayer_init(struct cubelayer *dst, int w, int h);
int cubelayer_free(struct cubelayer *dst);

int cubelayer_bind(struct cubelayer *dst);
int cubelayer_unbind(struct cubelayer *dst);


#endif //CUBELAYER_H
