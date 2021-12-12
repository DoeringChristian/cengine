#ifndef VERT_H
#define VERT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "cglm/cglm.h"

// screen space vertex
struct vert2{
    vec2 pos;
};

struct vert3{
    vec3 pos;
};

// cubvemap vertex
struct cmvert{
    vec3 pos;
    vec2 uv;
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
    vec2 tex_offset;
};

struct tri{
    GLuint idxs[3];
};

static struct tri tri(size_t v1_idx, size_t v2_idx, size_t v3_idx){
    return (struct tri){.idxs[0] = v1_idx, .idxs[1] = v2_idx, .idxs[2] = v3_idx};
}



#if 0
static struct vert2 vert2_quad[] = {
        {
            .pos = {-1, -1},
        },
        {
            .pos = {-1, 1},
        },
        {
            .pos = {1, -1},
        },
        {
            .pos = {1, 1},
        },
};

static GLint idxs_quad[] = {
    0, 1, 2,
    2, 3, 1,
};
static struct vert3 vert3_cube[] = {
    // front
    -1.0, -1.0,  1.0,
    1.0, -1.0,  1.0,
    1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
    1.0, -1.0, -1.0,
    1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0
};
static int idxs_cube[] = {
    // front
    2, 1, 0,
    0, 3, 2,
    // right
    6, 5, 1,
    1, 2, 6,
    // back
    5, 6, 7,
    7, 4, 5,
    // left
    3, 0, 4,
    4, 7, 3,
    // bottom
    1, 5, 4,
    4, 0, 1,
    // top
    6, 2, 3,
    3, 7, 6
};
#endif

#endif //VERT_H
