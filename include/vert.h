#ifndef VERT_H
#define VERT_H

#include <GL/glew.h>
#include <GL/gl.h>

// screen space vertex
struct svert{
    float pos[2];
    float uv[2];
};

// mesh vertex
struct vert{
    float pos[3];
    float normal[3];
    float color[4];
    float uv[2];
};

// light vertex
struct lvert{
    float pos[4];
    float color[4];
};

// instance vertex
struct ivert{
    float trans[16];
    float tex_idx_offset;
    float light_tex_idx;
};

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
