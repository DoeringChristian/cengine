#ifndef ENVMAP_H
#define ENVMAP_H

#include "layer.h"
#include "cubelayer.h"
#include "shader.h"

struct envmap{
    int w, h;

    struct texture hdr;
    struct texture irr;

    struct glbuf vbo;
    struct glbuf ibo;
    GLuint gl_vao, gl_fbo, gl_rbo;

    struct shader shader_eqr_to_cm;
};

int envmap_init(struct envmap *dst, int w, int h);
void envmap_free(struct envmap *dst);

int envmap_draw(struct envmap *dst, struct shader *shader, struct cvert *camera);

int envmap_draw_cube();

int envmap_hdr_set(struct envmap *dst, struct texture *src);

#endif //ENVMAP_H
