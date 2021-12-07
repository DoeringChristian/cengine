#ifndef GBUF_H
#define GBUF_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "glbuf.h"
#include "vert.h"
#include "glbuf.h"
#include "texture.h"
#include "shader.h"

struct gbuf{
    struct glbuf vbo;
    struct glbuf ibo;

    int w, h;

    GLuint gl_fbo;
    GLuint gl_vao;
    GLuint gl_rbo;

    struct texture pos, normal, color;

    //struct shader shader;
};

int gbuf_init(struct gbuf *dst, int w, int h);
void gbuf_free(struct gbuf *dst);

int gbuf_bind(struct gbuf *dst);
int gbuf_unbind(struct gbuf *dst);

int gbuf_draw(struct gbuf *dst, struct shader *shader, struct lvert light);

#endif //GBUF_H
