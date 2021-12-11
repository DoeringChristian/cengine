#ifndef CMESH_H
#define CMESH_H

#include "darray.h"
#include "vert.h"
#include "mesh.h"
#include "cglm/cglm.h"

/*
 * cmesh is a mesh that lives in ram
 */

struct cmesh{
    darray(struct vert) verts;
    darray(struct tri) tris;
    darray(struct ivert) iverts;

    struct mesh mesh;
};

int cmesh_init(struct cmesh *dst);
void cmesh_free(struct cmesh *dst);

void cmesh_push(struct cmesh *dst);

int cmesh_vert_push(struct cmesh *dst, struct vert src, size_t i);
int cmesh_vert_push_back(struct cmesh *dst, struct vert src);
int cmesh_vert_append(struct cmesh *dst, struct vert *src, size_t n);
int cmesh_vert_insert(struct cmesh *dst, struct vert *src, size_t i, size_t n);
int cmesh_vert_set(struct cmesh *dst, struct vert src, size_t i);
int cmesh_vert_setn(struct cmesh *dst, struct vert *src, size_t i, size_t n);

int cmesh_tri_push(struct cmesh *dst, struct tri src, size_t i);
int cmesh_tri_push_back(struct cmesh *dst, struct tri src);
int cmesh_tri_append(struct cmesh *dst, struct tri *src, size_t n);
int cmesh_tri_insert(struct cmesh *dst, struct tri *src, size_t i, size_t n);
int cmesh_tris_inc_cond(struct cmesh *dst, size_t i, size_t n);

int cmesh_append(struct cmesh *dst, const struct cmesh *src);

void cmesh_set_rot_axis(struct cmesh *dst, vec3 axis, float angle);
void cmesh_set_pos(struct cmesh *dst, vec3 pos);

#endif //CMESH_H
