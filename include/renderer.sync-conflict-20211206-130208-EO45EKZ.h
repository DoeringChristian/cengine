#ifndef RENDERER_H
#define RENDERER_H

#include "darray.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "mesh.h"
#include "shader.h"
#include "instance.h"

struct renderer{
    darray(struct instance *) instances;
    darray(struct mesh *) meshes;

    struct shader shader;
};

int renderer_init(struct renderer *dst);
void renderer_free(struct renderer *dst);

int renderer_push(struct renderer *dst);
int renderer_render(struct renderer *src);

int renderer_mesh_push(struct renderer *dst, struct mesh *src);
int renderer_instance_push(struct renderer *dst, struct instance *src);

#endif //RENDERER_H
