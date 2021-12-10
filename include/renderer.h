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

struct renderer{
    int w, h;

    darray(struct mesh *) meshes;
    darray(struct light *) lights;

    struct cvert camera;

    struct shader shader;

    struct layer gbuf;

    struct shader shader_forward;
    struct shader shader_shadow;
    struct shader shader_lighting;

    struct layer light;
    struct layer light_sum;
    struct layer light_tmp;
    struct cubelayer cl_shadow;
};

int renderer_init(struct renderer *dst, int w, int h);
void renderer_free(struct renderer *dst);

int renderer_render(struct renderer *src);
int renderer_render_point_shadow(struct renderer *src, struct light *light);

int renderer_mesh_register(struct renderer *dst, struct mesh *src);
int renderer_mesh_unregister(struct renderer *dst, struct mesh *target);
int renderer_light_register(struct renderer *dst, struct light *src);
int renderer_light_unregister(struct renderer *dst, struct light *target);

#endif //RENDERER_H
