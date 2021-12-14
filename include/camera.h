#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include "cglm/cglm.h"
#include "gl_util.h"

struct camera{
    mat4 proj;
    mat4 view;
    float far;
};

struct camera camera(int w, int h, float fov);
int camera_init(struct camera *dst, int w, int h, float fov);
int camera_init_cube(struct camera dst[6], vec3 pos, float far);
void camera_free(struct camera *dst);

void camera_translate(struct camera *dst, vec3 pos);
void camera_rotate(struct camera *dst, vec3 axis, float angle);

#endif //CAMERA_H
