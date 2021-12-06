#ifndef MESH_H
#define MESH_H

#include "darray.h"
#include "mathc.h"
#include "mathc_ext.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include "texture.h"
#include "shader.h"

#define MAX_TEXTURES 10

struct vert{
    float pos[3];
    float normal[3];
    float color[4];
    float uv[2];
};

struct tri{
    GLuint idxs[3];
};

struct ivert{
    float trans[16];
    float tex_idx_offset;
};

static struct tri tri(size_t v1_idx, size_t v2_idx, size_t v3_idx){
    return (struct tri){.idxs[0] = v1_idx, .idxs[1] = v2_idx, .idxs[2] = v3_idx};
}

struct mesh{
    darray(struct vert) verts;
    darray(struct tri) tris;
    darray(struct texture) textures;
    darray(struct ivert) instances;

    float pos[3]; 
    float rot[4]; //quaternion

    struct shader *shader;

    GLuint gl_vbo, gl_ibo, gl_vao;
    GLsizei gl_vbo_size;
    GLsizei gl_ibo_size;
    GLsizei gl_vboi_size;
    GLuint gl_vboi;
};

int mesh_init(struct mesh *dst, struct shader *shader);
void mesh_free(struct mesh *dst);

int mesh_update(struct mesh *dst);
int mesh_draw(struct mesh *src);

int mesh_attr_push(struct mesh *dst, GLsizei num, GLenum type, GLint normalized);
int mesh_vert_push_back(struct mesh *dst, struct vert src);
int mesh_vert_push(struct mesh *dst, struct vert src, size_t i);
int mesh_append(struct mesh *dst, const struct mesh *src);

int mesh_ivert_push_back(struct mesh *dst, struct ivert src);

int mesh_texture_push(struct mesh *dst, struct texture src);

int mesh_tri_push_back(struct mesh *dst, struct tri src);


#endif //MESH_H
