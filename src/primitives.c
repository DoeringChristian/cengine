#include "primitives.h"

int mesh_init_quad(struct mesh *dst){
    mesh_init(dst, primitive_verts_quad, 4, primitive_tris_quad, 2);

    return 0;
}
int mesh_init_cube(struct mesh *dst){
    
    mesh_init(dst, primitive_verts_cube, 24, primitive_tris_cube, 12);

    return 0;
}
int cmesh_init_quad(struct cmesh *dst){
    cmesh_init(dst);
    cmesh_vert_append(dst, primitive_verts_quad, 4);
    cmesh_tri_append(dst, primitive_tris_quad, 2);
    return 0;
}
int cmesh_init_cube(struct cmesh *dst){
    cmesh_init(dst);
    cmesh_vert_append(dst, primitive_verts_cube, 24);
    cmesh_tri_append(dst, primitive_tris_cube, 12);
    return 0;
}

