#ifndef CAMERA_H
#define CAMERA_H

#include "mathc.h"
#include "mathc_ext.h"
#include <stdio.h>

struct camera{
    float mat[16];
};

int camera_init(struct camera *dst, int w, int h, float fov);
void camera_free(struct camera *dst);

#endif //CAMERA_H
