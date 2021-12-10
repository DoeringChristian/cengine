#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "cglm/cglm.h"

// either as pointer in mesh or directly
struct material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    struct texture *map_albedo;
    struct texture *map_normal;
    struct texture *map_spec;
};

int material_init(struct material *dst);
void material_free(struct material *dst);

#endif //MATERIAL_H
