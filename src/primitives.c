#include "primitives.h"

struct mesh2 mesh2_quad;
struct mesh3 mesh3_cube;

struct shader primitive_shader_gbuf;
struct shader primitive_shader_quad_forward;
struct shader primitive_shader_shadow;
struct shader primitive_shader_lighting;
struct shader primitive_shader_emission;
struct shader primitive_shader_clip;
struct shader primitive_shader_bulh;
struct shader primitive_shader_blurh;
struct shader primitive_shader_blurv;
struct shader primitive_shader_gamma;
struct shader primitive_shader_skybox;
struct shader primitive_shader_ambient;
struct shader primitive_shader_eqr_to_cm;
struct shader primitive_shader_irr;
struct shader primitive_shader_ref;
struct shader primitive_shader_brdf_int;

const char shader_vert_quad_src[] = {
#include "vert_quad.glsl"
    0x00
};
const char shader_vert_cm_src[] = {
#include "vert_cm.glsl"
    0x00
};
const char shader_vert_src[] = {
#include "vert.glsl"
    0x00
};
const char shader_vert_shadow_src[] = {
#include "vert_shadow.glsl"
    0x00
};

const char shader_frag_src[] = {
#include "frag.glsl"
    0x00
};
const char shader_frag_forward_src[] = {
#include "frag_forward.glsl"
    0x00
};
const char shader_frag_shadow_src[] = {
#include "frag_shadow.glsl"
    0x00
};
const char shader_frag_lighting_src[] = {
#include "frag_lighting.glsl"
    0x00
};
const char shader_frag_emission_src[] = {
#include "frag_emission.glsl"
    0x00
};
const char shader_frag_clip_src[] = {
#include "frag_clip.glsl"
    0x00
};
const char shader_frag_blurh_src[] = {
#include "frag_blurh.glsl"
    0x00
};
const char shader_frag_blurv_src[] = {
#include "frag_blurv.glsl"
    0x00
};
const char shader_frag_gamma_correct_src[] = {
#include "frag_gamma_correct.glsl"
    0x00
};
const char shader_frag_skybox_src[] = {
#include "frag_skybox.glsl"
    0x00
};
const char shader_frag_ambient_src[] = {
#include "frag_ambient.glsl"
    0x00
};
const char shader_frag_eqr_to_cm_src[] = {
#include "frag_eqr_to_cm.glsl"
    0x00
};
const char shader_frag_irr_src[] = {
#include "frag_irr.glsl"
    0x00
};
const char shader_frag_ref_src[] = {
#include "frag_ref.glsl"
    0x00
};
const char shader_frag_brdf_int_src[] = {
#include "frag_brdf_int.glsl"
    0x00
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

    // load primitive meshes
    mesh2_init(&mesh2_quad, 
            vert2_quad, sizeof(vert2_quad)/sizeof(struct vert2), 
            idxs_quad, sizeof(idxs_quad)/sizeof(int));
    mesh3_init(&mesh3_cube, 
            vert3_cube, sizeof(vert3_cube)/sizeof(struct vert3), 
            idxs_cube, sizeof(idxs_cube)/sizeof(int));

    // load primitive shaders
    shader_init_src(&primitive_shader_gbuf, shader_vert_src, shader_frag_src);
    shader_init_src(&primitive_shader_quad_forward, shader_vert_quad_src, shader_frag_forward_src);
    shader_init_src(&primitive_shader_shadow, shader_vert_shadow_src, shader_frag_shadow_src);
    shader_init_src(&primitive_shader_lighting, shader_vert_quad_src, shader_frag_lighting_src);
    shader_init_src(&primitive_shader_emission, shader_vert_quad_src, shader_frag_emission_src);
    shader_init_src(&primitive_shader_clip, shader_vert_quad_src, shader_frag_clip_src);
    shader_init_src(&primitive_shader_blurh, shader_vert_quad_src, shader_frag_blurh_src);
    shader_init_src(&primitive_shader_blurv, shader_vert_quad_src, shader_frag_blurv_src);
    shader_init_src(&primitive_shader_gamma, shader_vert_quad_src, shader_frag_gamma_correct_src);
    shader_init_src(&primitive_shader_skybox, shader_vert_cm_src, shader_frag_skybox_src);
    shader_init_src(&primitive_shader_ambient, shader_vert_quad_src, shader_frag_ambient_src);
    shader_init_src(&primitive_shader_eqr_to_cm, shader_vert_cm_src, shader_frag_eqr_to_cm_src);
    shader_init_src(&primitive_shader_irr, shader_vert_cm_src, shader_frag_irr_src);
    shader_init_src(&primitive_shader_ref, shader_vert_cm_src, shader_frag_ref_src);
    shader_init_src(&primitive_shader_brdf_int, shader_vert_quad_src, shader_frag_brdf_int_src);

    primitives_initialized = 1;
    return 0;
}
void primitives_free(){
    mesh2_free(&mesh2_quad);
    mesh3_free(&mesh3_cube);
    primitives_initialized = 0;
}
