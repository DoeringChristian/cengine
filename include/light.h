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
};

struct light light(vec4 pos, vec4 color, enum light_type type);

#endif //LIGHT_H
