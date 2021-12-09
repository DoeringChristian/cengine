#ifndef MESH_H
#define MESH_H

#include "darray.h"
#include "mathc_ext.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include "texture.h"
#include "shader.h"
#include "glbuf.h"
#include "vert.h"
#include "camera.h"
#include "cmesh.h"
#include "light.h"

#define MAX_TEXTURES 10
#define MAX_LIGHTS 4

/*
 * mesh is a mesh that lives on the gpu
 */


struct mesh{
    darray(struct texture) textures;

    vec3 pos;
    versor rot;

    //struct shader *shader;

    struct glbuf vbo;
    struct glbuf ibo;
    struct glbuf vboi;

    GLuint gl_vao;

    struct texture *tex_albedo;
    struct texture *tex_normal;
    struct texture *tex_spec;
};

int mesh_init(struct mesh *dst, struct vert *verts, size_t verts_len, struct tri *tris, size_t tris_len);
int mesh_init_cmesh(struct mesh *dst, struct cmesh *src);
void mesh_free(struct mesh *dst);

int mesh_push(struct mesh *dst);
int mesh_draw(struct mesh *src, struct cvert *camera, struct shader *shader);
// draw shadow cube map
int mesh_draw_depth(struct mesh *src, struct cvert *camera, struct shader *shader, struct light *light);

int mesh_vert_push_back(struct mesh *dst, struct vert src);
int mesh_vert_push(struct mesh *dst, struct vert src, size_t i);
int mesh_vert_append(struct mesh *dst, struct vert *src, size_t n);
int mesh_vert_set(struct mesh *dst, struct vert src, size_t i);
int mesh_vert_setn(struct mesh *dst, struct vert *src, size_t n, size_t i);

int mesh_append(struct mesh *dst, const struct mesh *src);

int mesh_ivert_push_back(struct mesh *dst, struct ivert src);
int mesh_iverts_clear(struct mesh *dst);

int mesh_texture_push(struct mesh *dst, struct texture src);
void mesh_texture_albedo_set(struct mesh *dst, struct texture *src);
void mesh_texture_normal_set(struct mesh *dst, struct texture *src);
void mesh_texture_spec_set(struct mesh *dst, struct texture *src);

int mesh_tri_push_back(struct mesh *dst, struct tri src);
int mesh_tri_append(struct mesh *dst, struct tri *src, size_t n);


#endif //MESH_H
