#include "material.h"

int material_init(struct material *dst){
    dst->albedo_map = NULL;
    dst->normal_map = NULL;
    dst->mrao_map = NULL;
    dst->emission_map = NULL;
    dst->normal_scale = 0.0;

    glm_vec4_copy(vec4(1, 0, 0, 1), dst->albedo);
    glm_vec4_copy(vec4(0.5, 0.5, 1, 1), dst->mrao);
    glm_vec4_copy(vec4(0, 0, 0, 0), dst->emission);

    resource_handle_init(&dst->handle, NULL, NULL);

    return 0;
}
void material_free(struct material *dst){
    resource_handle_free(&dst->handle);
}

void material_albedo_map_set(struct material *dst, struct texture *src){
    dst->albedo_map = src;
}
void material_normal_map_set(struct material *dst, struct texture *src, float normal_scale){
    dst->normal_map = src;
    dst->normal_scale = normal_scale;
}
void material_mrao_map_set(struct material *dst, struct texture *src){
    dst->mrao_map = src;
}
void material_emission_map_set(struct material *dst, struct texture *src){
    dst->emission_map = src;
}
