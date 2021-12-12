#pragma once
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
#include "light.h"
#include "camera.h"
#include "envmap.h"

struct envmap;

// eventualy use global definitions for primitives
static GLuint layer_gl_vao = 0;
static struct glbuf layer_ibo_plane;
static struct glbuf layer_vbo_plane;
static struct glbuf layer_ibo_cube;
static struct glbuf layer_vbo_cube;

struct layer{
    struct glbuf vbo;
    struct glbuf ibo;

    int w, h;

    GLuint gl_fbo;
    GLuint gl_vao;
    GLuint gl_rbo;

    //struct texture texture;
    darray(struct texture) textures;
};

int layer_init(struct layer *dst, int w, int h);
int layer_init_n(struct layer *dst, int w, int h, int num_textures);
int layer_init_env(struct layer *dst, int w, int h);
void layer_free(struct layer *dst);

int layer_clear(struct layer *dst);

int layer_bind(struct layer *dst);
int layer_rebind(struct layer *dst);
int layer_unbind(struct layer *dst);

int layer_draw(struct layer *dst, struct shader *shader);
int layer_draw_n(struct layer *dst, struct shader *shader);
int layer_draw_gbuf(struct layer *src, struct shader *shader, struct texture *shadow_depth, struct light *light, struct envmap *env, struct cvert *camera);
int layer_draw_gbuf_ambient(struct layer *src, struct shader *shader, struct envmap *env, struct cvert *camera);
int layer_draw_tex(struct layer *dst, struct shader *shader, struct texture *tex);

//int layer_blend(struct layer *dst, struct layer *src1, struct layer *src2, struct shader *bshader);

#endif //LAYER_H
