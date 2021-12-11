#include "material.h"

int material_init(struct material *dst){
    dst->albedo_map = NULL;
    dst->normal_map = NULL;
    dst->mrao_map = NULL;
    dst->emission_map = NULL;

    glm_vec4_copy(vec4(1, 0, 0, 1), dst->albedo);
    glm_vec4_copy(vec4(0.5, 0.5, 1, 1), dst->mrao);
    glm_vec4_copy(vec4(0, 0, 0, 0), dst->emission);

    return 0;
}
void material_free(struct material *dst){

}

void material_map_albedo_set(struct material *dst, struct texture *src){
    dst->albedo_map = src;
}
void material_map_normal_set(struct material *dst, struct texture *src){
    dst->normal_map = src;
}
void material_map_mrao_set(struct material *dst, struct texture *src){
    dst->mrao_map = src;
}
void material_map_emission_set(struct material *dst, struct texture *src){
    dst->emission_map = src;
}
