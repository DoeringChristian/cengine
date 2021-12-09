#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "cmesh.h"
#include "texture.h"
#include "darray.h"

struct model{
    darray(struct texture) textures;
    darray(struct mesh) meshes;
    darray(struct cmesh) cmeshes;
};

int model_load(struct model *dst, const char *path);

#endif //MODEL_H
