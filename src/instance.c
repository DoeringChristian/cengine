#include "instance.h"

int instance_init(struct instance *dst, struct mesh *src){
    dst->mesh = src;

    //mat4_identity(dst->ivert.trans);
    dst->ivert.tex_idx_offset = 0;

    return 0;
}
void instance_free(struct instance *dst){
    dst->mesh = NULL;
}

int instance_push(struct instance *dst){
    return mesh_ivert_push_back(dst->mesh, dst->ivert);
}
