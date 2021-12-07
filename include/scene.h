#ifndef SCENEH_H
#define SCENEH_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "mesh.h"
#include "darray.h"

struct scene{
    darray(struct mesh *) meshes;

    struct shader *shader;

    struct texture lights;
};

int scene_init(struct scene *dst);
void scene_free(struct scene *dst);

int scene_mesh_push_back(struct scene *dst, struct mesh *src);

int scene_draw(struct scene *dst);

#endif //SCENEH_H
