#ifndef SCENEH_H
#define SCENEH_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "mesh.h"
#include "darray.h"
#include "camera.h"

struct scene{
    darray(struct mesh *) meshes;
    darray(struct lvert) lights;

    struct shader *shader;
};

int scene_init(struct scene *dst);
void scene_free(struct scene *dst);

int scene_mesh_push_back(struct scene *dst, struct mesh *src);

int scene_draw(struct scene *dst, struct cvert *camera, struct shader *shader);
int scene_draw_shadow_cm(struct scene *dst, struct cvert *camera, struct shader *shader, struct lvert *light);

int scene_lvert_push(struct scene *dst, struct lvert src);

#endif //SCENEH_H
