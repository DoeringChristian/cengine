#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include "cglm/cglm.h"

struct camera{
    mat4 mat;
};

int camera_init(struct camera *dst, int w, int h, float fov);
void camera_free(struct camera *dst);

#endif //CAMERA_H
