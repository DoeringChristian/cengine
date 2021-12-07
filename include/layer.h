#ifndef LAYER_H
#define LAYER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include "glbuf.h"
#include "texture.h"
#include "shader.h"
#include "vert.h"

struct layer{
    struct glbuf vbo;
    struct glbuf ibo;

    int w, h;

    GLuint gl_fbo;
    GLuint gl_vao;
    GLuint gl_rbo;

    struct texture texture;
};

int layer_init(struct layer *dst, int w, int h, const char *path_vert, const char *path_frag);
void layer_free(struct layer *dst);

int layer_bind(struct layer *dst);
int layer_unbind(struct layer *dst);

int layer_draw(struct layer *dst, struct shader *shader);

int layer_blend(struct layer *dst, struct layer *src1, struct layer *src2, struct shader *bshader);

#endif //LAYER_H
