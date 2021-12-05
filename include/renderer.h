#ifndef RENDERER_H
#define RENDERER_H

#include "darray.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "mesh.h"
#include "shader.h"

struct renderer{
    darray(struct mesh *) meshes;

    struct shader shader;
};

int renderer_init(struct renderer *dst);
void renderer_free(struct renderer *dst);

int renderer_render(struct renderer *src);

int renderer_mesh_push(struct renderer *dst, struct mesh *src);

#endif //RENDERER_H
