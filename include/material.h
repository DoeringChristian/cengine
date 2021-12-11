#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "cglm/cglm.h"

// either as pointer in mesh or directly
struct material{
    vec4 albedo;
    vec4 mrao;
    vec4 emission;

    struct texture *albedo_map;
    struct texture *normal_map;
    // map metallic, roughness, ambient occlusion
    struct texture *mrao_map;
    struct texture *emission_map;
};

int material_init(struct material *dst);
//void material_free(struct material *dst);
void material_map_albedo_set(struct material *dst, struct texture *src);
void material_map_normal_set(struct material *dst, struct texture *src);
void material_map_mrao_set(struct material *dst, struct texture *src);
void material_map_emission_set(struct material *dst, struct texture *src);

#endif //MATERIAL_H
