#ifndef LIGHT_H
#define LIGHT_H

#include "cglm/cglm.h"

enum light_type{
    LIGHT_POINT,
};

struct light{
    vec4 pos;
    vec4 color;

    enum light_type type;

    float shadow_len;

    int casts_shadow;
};

int light_init(struct light *dst, vec4 pos, vec4 color, enum light_type type);

#endif //LIGHT_H
