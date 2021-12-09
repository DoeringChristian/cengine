#ifndef GBUF_H
#define GBUF_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "glbuf.h"
#include "vert.h"
#include "glbuf.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

struct gbuf{
    struct glbuf vbo;
    struct glbuf ibo;

    int w, h;

    GLuint gl_fbo;
    GLuint gl_vao;
    GLuint gl_rbo;

    struct texture pos, normal, color;
};

int gbuf_init(struct gbuf *dst, int w, int h);
void gbuf_free(struct gbuf *dst);

int gbuf_bind(struct gbuf *dst);
int gbuf_unbind(struct gbuf *dst);

int gbuf_draw(struct gbuf *dst, struct shader *shader, struct lvert light, struct texture *shadow_depth, float shadow_scale, struct cvert *camera);

int gbuf_shader_load(struct gbuf *dst, const char *vert_path, const char *frag_path);

#endif //GBUF_H
