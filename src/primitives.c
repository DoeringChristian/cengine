#include "primitives.h"

struct mesh2 mesh2_quad;
struct mesh3 mesh3_cube;

int mesh_init_quad(struct mesh *dst){
    mesh_init(dst, vert_quad, 4, tri_quad, 2);

    return 0;
}
int mesh_init_cube(struct mesh *dst){
    
    mesh_init(dst, vert_cube, 24, tri_cube, 12);

    return 0;
}
int cmesh_init_quad(struct cmesh *dst){
    cmesh_init(dst);
    cmesh_vert_append(dst, vert_quad, 4);
    cmesh_tri_append(dst, tri_quad, 2);
    return 0;
}
int cmesh_init_cube(struct cmesh *dst){
    cmesh_init(dst);
    cmesh_vert_append(dst, vert_cube, 24);
    cmesh_tri_append(dst, tri_cube, 12);
    return 0;
}

int primitives_init(){
    if(primitives_initialized)
        return 0;
    mesh2_init(&mesh2_quad, 
            vert2_quad, sizeof(vert2_quad)/sizeof(struct vert2), 
            idxs_quad, sizeof(idxs_quad)/sizeof(int));
    mesh3_init(&mesh3_cube, 
            vert3_cube, sizeof(vert3_cube)/sizeof(struct vert3), 
            idxs_cube, sizeof(idxs_cube)/sizeof(int));


    primitives_initialized = 1;
    return 0;
}
void primitives_free(){
    mesh2_free(&mesh2_quad);
    mesh3_free(&mesh3_cube);
    primitives_initialized = 0;
}
