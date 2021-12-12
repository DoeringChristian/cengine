#include "primitives.h"

struct mesh2 mesh2_quad;
struct mesh3 mesh3_cube;

const char shader_vert_quad_src[] = {
#include "vert_quad.glsl"
};
const char shader_vert_cm_src[] = {
#include "vert_cm.glsl"
};
const char shader_vert_src[] = {
#include "vert.glsl"
};
const char shader_vert_shadow_src[] = {
#include "vert_shadow.glsl"
};

const char shader_frag_src[] = {
#include "frag.glsl"
};
const char shader_frag_forward_src[] = {
#include "frag_forward.glsl"
};
const char shader_frag_shadow_src[] = {
#include "frag_shadow.glsl"
};
const char shader_frag_lighting_src[] = {
#include "frag_lighting.glsl"
};
const char shader_frag_emission_src[] = {
#include "frag_emission.glsl"
};
const char shader_frag_clib_src[] = {
#include "frag_clip.glsl"
};
const char shader_frag_blurh_src[] = {
#include "frag_blurh.glsl"
};
const char shader_frag_blurv_src[] = {
#include "frag_blurv.glsl"
};
const char shader_frag_gamma_correct_src[] = {
#include "frag_gamma_correct.glsl"
};
const char shader_frag_skybox_src[] = {
#include "frag_skybox.glsl"
};
const char shader_frag_ambient_src[] = {
#include "frag_ambient.glsl"
};
const char shader_frag_eqr_to_cm_src[] = {
#include "frag_eqr_to_cm.glsl"
};
const char shader_frag_irr_src[] = {
#include "frag_irr.glsl"
};
const char shader_frag_ref_src[] = {
#include "frag_ref.glsl"
};
const char shader_frag_brdf_int_src[] = {
#include "frag_brdf_int.glsl"
};

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
