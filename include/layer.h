#ifndef LAYER_H
#define LAYER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include "glbuf.h"
#include "texture.h"
#include "shader.h"
#include "vert.h"
#include "darray.h"
#include "gbuf.h"

struct layer{
    struct glbuf vbo;
    struct glbuf ibo;

    int w, h;

    GLuint gl_fbo;
    GLuint gl_vao;
    GLuint gl_rbo;

    struct texture texture;
    struct shader shader;
};

int layer_init(struct layer *dst, int w, int h);
int layer_init_shader(struct layer *dst, int w, int h, const char *vert_path, const char *frag_path);
void layer_free(struct layer *dst);

int layer_clear(struct layer *dst);

int layer_bind(struct layer *dst);
int layer_unbind(struct layer *dst);

int layer_draw(struct layer *dst);
int layer_draw_shader(struct layer *dst, struct shader *shader);

int layer_blend(struct layer *dst, struct layer *src1, struct layer *src2, struct shader *bshader);
int layer_blend_gbuf(struct layer *dst, struct layer *srcl, struct gbuf *srcg, struct shader *bshader);

int layer_shader_load(struct layer *dst, const char *vert_path, const char *frag_path);

#endif //LAYER_H
