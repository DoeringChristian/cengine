#ifndef CUBELAYER_H
#define CUBELAYER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include "glbuf.h"
#include "texture.h"
#include "vert.h"
#include "shader.h"

struct cubelayer{
    struct glbuf vbo;
    struct glbuf ibo;

    int w, h;

    GLuint gl_fbo;
    GLuint gl_vao;
    GLuint gl_rbo;
    GLenum attachment;

    struct texture texture;
};

int cubelayer_init_rgbf16(struct cubelayer *dst, int w, int h);
int cubelayer_init_depthcube(struct cubelayer *dst, int w, int h);
int cubelayer_free(struct cubelayer *dst);

int cubelayer_bind(struct cubelayer *dst, int face);
int cubelayer_unbind(struct cubelayer *dst);
int cubelayer_draw(struct cubelayer *dst, struct shader *shader);

#endif //CUBELAYER_H
