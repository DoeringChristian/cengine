#ifndef LIGHT_H
#define LIGHT_H

#include "cglm/cglm.h"
#include "resource_handle.h"

enum light_type{
    LIGHT_POINT,
};

struct light{
    struct resource_handle handle;
    vec4 pos;
    vec4 color;

    enum light_type type;

    float shadow_len;

    int casts_shadow;
};

int light_init(struct light *dst, vec4 pos, vec4 color, enum light_type type);
void light_free(struct light *dst);

#endif //LIGHT_H
