#ifndef RENDERER_H
#define RENDERER_H

#include "darray.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "mesh.h"
#include "shader.h"
#include "scene.h"

struct renderer{
    struct scene *scene;

    struct shader shader;
};

int renderer_init(struct renderer *dst);
void renderer_free(struct renderer *dst);

int renderer_push(struct renderer *dst);
int renderer_render(struct renderer *src);

int renderer_scene_set(struct renderer *dst, struct scene *src);

#endif //RENDERER_H
