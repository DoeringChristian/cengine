#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include "cglm/cglm.h"
#include "gl_util.h"

struct cvert{
    mat4 proj;
    mat4 view;
    float far;
};

struct cvert cvert(int w, int h, float fov);
int cvert_init(struct cvert *dst, int w, int h, float fov);
int cverts_init_cube(struct cvert dst[6], vec3 pos, float far);
void camera_free(struct cvert *dst);

void cvert_translate(struct cvert *dst, vec3 pos);
void cvert_rotate(struct cvert *dst, vec3 axis, float angle);

#endif //CAMERA_H
