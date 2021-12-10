#ifndef VERT_H
#define VERT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "cglm/cglm.h"

// screen space vertex
struct svert{
    vec2 pos;
    vec2 uv;
#if 0
    float pos[2];
    float uv[2];
#endif
};

// mesh vertex
struct vert{
    vec3 pos;
    vec3 normal;
    vec3 tangent;
    vec4 color;
    vec2 uv;
};

// light vertex
struct lvert{
    vec4 pos;
    vec4 color;
};

// instance vertex
struct ivert{
    mat4 trans;
    float tex_idx_offset;
};



struct tri{
    GLuint idxs[3];
};

static struct tri tri(size_t v1_idx, size_t v2_idx, size_t v3_idx){
    return (struct tri){.idxs[0] = v1_idx, .idxs[1] = v2_idx, .idxs[2] = v3_idx};
}



static struct svert svert_quad[] = {
        {
            .pos = {-1, -1},
            .uv = {0, 0},
        },
        {
            .pos = {-1, 1},
            .uv = {0, 1},
        },
        {
            .pos = {1, -1},
            .uv = {1, 0},
        },
        {
            .pos = {1, 1},
            .uv = {1, 1},
        },
};

static GLint idxs_quad[] = {
    0, 1, 2,
    2, 3, 1,
};

#endif //VERT_H
