#include "light.h"

struct light light(vec4 pos, vec4 color, enum light_type type){
    struct light dst;
    glm_vec4_copy(pos, dst.pos);
    glm_vec4_copy(color, dst.color);
    dst.shadow_len = 100;
    return dst;
}
