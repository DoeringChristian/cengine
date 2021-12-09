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
#include "light.h"

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

/*
 * Draws the GBuffer to the currently active framebuffer with a point light.
 *
 * @param src: gbufer to draw
 * @param shader: Shader to use when drawing the gbuf.
 * @param shadow_depth: cubemap texture specifying the distance to geometry from the light source.
 * @param light_prev: Comulative result of the prvious passes.
 * @param light: light source.
 * @param camera: camera from which to view the gbuf.
 */
int gbuf_draw_ps(struct gbuf *src, struct shader *shader, struct texture *shadow_depth, struct texture *light_prev, struct light *light, struct cvert *camera);

int gbuf_shader_load(struct gbuf *dst, const char *vert_path, const char *frag_path);

#endif //GBUF_H
