#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "cglm/cglm.h"

// either as pointer in mesh or directly
struct material{
    vec4 albedo;
    vec4 mrao;
    vec4 emission;
    float normal_scale;

    struct texture *albedo_map;
    struct texture *normal_map;
    // map metallic, roughness, ambient occlusion
    struct texture *mrao_map;
    struct texture *emission_map;
};

int material_init(struct material *dst);
//void material_free(struct material *dst);
void material_albedo_map_set(struct material *dst, struct texture *src);
void material_normal_map_set(struct material *dst, struct texture *src, float normal_scale);
void material_mrao_map_set(struct material *dst, struct texture *src);
void material_emission_map_set(struct material *dst, struct texture *src);

void material_free(struct material *dst);

#endif //MATERIAL_H
