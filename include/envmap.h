#pragma once
#ifndef ENVMAP_H
#define ENVMAP_H

#include "layer.h"
#include "cubelayer.h"
#include "shader.h"

struct envmap{
    int res;
    int mmlvl;

    struct texture hdr;
    struct texture irr;
    struct texture ref;
    struct texture brdf_int;

    GLuint gl_fbo, gl_rbo;

    struct shader *shader_eqr_to_cm;
    struct shader *shader_irr;
    struct shader *shader_ref;
    struct shader *shader_brdf_int;
};

int envmap_init(struct envmap *dst, int w);
void envmap_free(struct envmap *dst);

int envmap_draw(struct envmap *dst, struct shader *shader, struct camera *camera);

int envmap_draw_cube();

int envmap_hdr_set(struct envmap *dst, struct texture *src);

int envmap_calc_brdf_lut(struct envmap *dst);
int envmap_calc_hdr_cm(struct envmap *dst, struct texture *src);

#endif //ENVMAP_H
