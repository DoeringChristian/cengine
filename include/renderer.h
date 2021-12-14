#ifndef RENDERER_H
#define RENDERER_H

#include "darray.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "mesh.h"
#include "shader.h"
#include "scene.h"
#include "layer.h"
#include "camera.h"
#include "cubelayer.h"
#include "envmap.h"

struct renderer{
    int w, h;

    darray(struct mesh *) meshes;
    darray(struct light *) lights;

    struct camera camera;

    // shaders
    struct shader *shader;
    struct shader *shader_forward;
    struct shader *shader_shadow;
    struct shader *shader_lighting;
    struct shader *shader_clip;
    struct shader *shader_blurh;
    struct shader *shader_blurv;
    struct shader *shader_gamma;
    struct shader *shader_emission;
    struct shader *shader_skybox;
    struct shader *shader_ambient;

    //struct layer light;
    struct layer light_out;
    struct layer layer_bloom;
    struct layer gbuf;

    struct cubelayer cl_shadow;
    //struct cubelayer cl_hdr;

    struct envmap environment;

    vec4 ambient;
};

int renderer_init(struct renderer *dst, int w, int h);
void renderer_free(struct renderer *dst);

int renderer_render(struct renderer *src);
int renderer_render_point_shadow(struct renderer *src, struct light *light);
int renderer_render_bloom(struct renderer *src);

int renderer_mesh_register(struct renderer *dst, struct mesh *src);
int renderer_mesh_unregister(struct renderer *dst, struct mesh *target);
int renderer_light_register(struct renderer *dst, struct light *src);
int renderer_light_unregister(struct renderer *dst, struct light *target);

int renderer_envmap_set(struct renderer *dst, struct texture *hdri);

#endif //RENDERER_H
