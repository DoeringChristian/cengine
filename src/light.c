#include "light.h"

int light_init(struct light *dst, vec4 pos, vec4 color, enum light_type type){
    resource_handle_init(&dst->handle, NULL, NULL);
    glm_vec4_copy(pos, dst->pos);
    glm_vec4_copy(color, dst->color);
    dst->shadow_len = 100;
    dst->casts_shadow = 1;
    dst->type = LIGHT_POINT;
    return 0;
}
void light_free(struct light *dst){
    resource_handle_free(&dst->handle);
}
