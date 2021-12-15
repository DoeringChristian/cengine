#ifndef INSTANCER_H
#define INSTANCER_H

#include "cglm/cglm.h"
#include "mesh.h"
#include "vert.h"

struct instance{
    struct mesh *mesh;
    struct ivert ivert;
};

struct instancer{
    darray(struct instance *) instances;
};

int instance_init(struct instance *dst, struct mesh *mesh, vec3 pos, float angle, vec3 axis, vec3 scale);

int instancer_init(struct instancer *dst);
void instancer_free(struct instancer *dst);

int instancer_update(struct instancer *dst);
int instancer_instance_push(struct instancer *dst, struct instance *src);
int instancer_instance_pop(struct instancer *dst, struct instance *target);

#endif //INSTANCER_H
