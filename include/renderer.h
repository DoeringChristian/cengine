#ifndef RENDERER_H
#define RENDERER_H

#include "darray.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "mesh.h"
#include "shader.h"
#include "scene.h"
#include "layer.h"
#include "gbuf.h"
#include "camera.h"

struct renderer{
    int w, h;

    struct scene *scene;

    struct cvert camera;

    struct shader shader;

    struct gbuf gbuf;

    struct shader shader_add;
    struct shader shader_mul;
    struct shader shader_forward;
    struct shader shader_shadow;

    struct layer light;
    struct layer light_sum;
    struct layer light_tmp;
    struct layer shadow_cube;
};

int renderer_init(struct renderer *dst, int w, int h);
void renderer_free(struct renderer *dst);

int renderer_push(struct renderer *dst);
int renderer_render(struct renderer *src);

int renderer_scene_set(struct renderer *dst, struct scene *src);

#endif //RENDERER_H
