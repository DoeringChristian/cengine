#include "cmesh.h"

int cmesh_init(struct cmesh *dst){
    darray_init(&dst->verts, 100);
    darray_init(&dst->tris, 100);
    
    dst->pos[0] = 0;
    dst->pos[1] = 0;
    dst->pos[2] = 0;

    dst->rot[0] = 0;
    dst->rot[1] = 0;
    dst->rot[2] = 0;
    dst->rot[3] = 1;
    return 0;
}
void cmesh_free(struct cmesh *dst){
    darray_free(&dst->verts);
    darray_free(&dst->tris);
}

int cmesh_vert_push(struct cmesh *dst, struct vert src, size_t i){
    cmesh_tris_inc_cond(dst, i, 1);
    return darray_push(&dst->verts, src, i);
}
int cmesh_vert_push_back(struct cmesh *dst, struct vert src){
    return darray_push_back(&dst->verts, src);
}
int cmesh_vert_append(struct cmesh *dst, struct vert *src, size_t n){
    return darray_append(&dst->verts, src, n);
}
int cmesh_vert_insert(struct cmesh *dst, struct vert *src, size_t i, size_t n){
    cmesh_tris_inc_cond(dst, i, n);
    return darray_insert(&dst->verts, src, n, i);
}

int cmesh_tri_push(struct cmesh *dst, struct tri src, size_t i){
    return darray_push(&dst->tris, src, i);
}
int cmesh_tri_push_back(struct cmesh *dst, struct tri src){
    return darray_push_back(&dst->tris, src);
}
int cmesh_tri_append(struct cmesh *dst, struct tri *src, size_t n){
    return darray_append(&dst->tris, src, n);
}
int cmesh_tri_insert(struct cmesh *dst, struct tri *src, size_t i, size_t n){
    return darray_insert(&dst->tris, src, n, i);
}
int cmesh_tris_inc_cond(struct cmesh *dst, size_t i, size_t n){
    for(size_t i = 0;i < darray_len(&dst->tris);i++){
        if(dst->tris[i].idxs[0] <= i)
            dst->tris[i].idxs[0] += n;
        if(dst->tris[i].idxs[1] <= i)
            dst->tris[i].idxs[1] += n;
        if(dst->tris[i].idxs[2] <= i)
            dst->tris[i].idxs[2] += n;
    }
    return 0;
}

int cmesh_append(struct cmesh *dst, const struct cmesh *src){
    size_t dst_vert_len = darray_len(&dst->verts);
    struct vert dst_vert;
    for(size_t i = 0;i < darray_len(&src->verts);i++){
        dst_vert = src->verts[i];
        
        versor dst_rot_inv;
        glm_quat_inv(dst->rot, dst_rot_inv);

        vec3 tmpv3;

        glm_vec3_add(dst_vert.pos, (float *)src->pos, dst_vert.pos);
        glm_vec3_sub(dst_vert.pos, dst->pos, dst_vert.pos);

        glm_quat_rotatev((float *)src->rot, dst_vert.pos, tmpv3);
        glm_quat_rotatev(dst_rot_inv, tmpv3, dst_vert.pos);

        glm_quat_rotatev((float *)src->rot, dst_vert.normal, tmpv3);
        glm_quat_rotatev(dst_rot_inv, tmpv3, dst_vert.normal);
        
        cmesh_vert_push_back(dst, dst_vert);
    }

    for(size_t i = 0;i < darray_len(&src->tris);i++){
        struct tri dst_tri = {
            {
                src->tris[i].idxs[0] + dst_vert_len,
                src->tris[i].idxs[1] + dst_vert_len,
                src->tris[i].idxs[2] + dst_vert_len,
            }
        };
        cmesh_tri_push_back(dst, dst_tri);
    }
    return 0;
}

void cmesh_set_rot_axis(struct cmesh *dst, vec3 axis, float angle){
    glm_quatv(dst->rot, angle, axis);
}
void cmesh_set_pos(struct cmesh *dst, vec3 pos){
    glm_vec3_copy(pos, dst->pos);
}
