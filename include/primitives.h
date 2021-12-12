#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "mesh.h"
#include "cmesh.h"

int mesh_init_quad(struct mesh *dst);
int mesh_init_cube(struct mesh *dst);

int cmesh_init_quad(struct cmesh *dst);
int cmesh_init_cube(struct cmesh *dst);

static struct vert primitive_verts_quad[] = {
        {
            .pos = {-1, -1, 0},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {0, 0},
        },
        {
            .pos = {-1, 1, 0},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {0, 1},
        },
        {
            .pos = {1, -1, 0},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {1, 0},
        },
        {
            .pos = {1, 1, 0},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {1, 1},
        },
};

static struct tri primitive_tris_quad[] = {
        {
            0, 1, 2,
        },
        {
            2, 3, 1,
        },
};

static struct vert primitive_verts_cube[] = {
        // side 0
        {
            .pos = {-1, -1, -1},
            .normal = {0, 0, -1},
            .color = {0, 0, 0, 1},
            .uv = {0, 0},
        },
        {
            .pos = {-1, 1, -1},
            .normal = {0, 0, -1},
            .color = {0, 0, 0, 1},
            .uv = {0, 1},
        },
        {
            .pos = {1, -1, -1},
            .normal = {0, 0, -1},
            .color = {0, 0, 0, 1},
            .uv = {1, 0},
        },
        {
            .pos = {1, 1, -1},
            .normal = {0, 0, -1},
            .color = {0, 0, 0, 1},
            .uv = {1, 1},
        },

        // side 1

        {
            .pos = {-1, -1, 1},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {0, 0},
        },
        {
            .pos = {-1, 1, 1},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {0, 1},
        },
        {
            .pos = {1, -1, 1},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {1, 0},
        },
        {
            .pos = {1, 1, 1},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {1, 1},
        },

        // side 2

        {
            .pos = {-1, -1, -1},
            .normal = {-1, 0, 0},
            .color = {0, 0, 0, 1},
            .uv = {0, 0},
        },
        {
            .pos = {-1, -1, 1},
            .normal = {-1, 0, 0},
            .color = {0, 0, 0, 1},
            .uv = {0, 1},
        },
        {
            .pos = {-1, 1, -1},
            .normal = {-1, 0, 0},
            .color = {0, 0, 0, 1},
            .uv = {1, 0},
        },
        {
            .pos = {-1, 1, 1},
            .normal = {-1, 0, 0},
            .color = {0, 0, 0, 1},
            .uv = {1, 1},
        },

        // side 3

        {
            .pos = {1, -1, -1},
            .normal = {1, 0, 0},
            .color = {0, 0, 0, 1},
            .uv = {0, 0},
        },
        {
            .pos = {1, -1, 1},
            .normal = {1, 0, 0},
            .color = {0, 0, 0, 1},
            .uv = {0, 1},
        },
        {
            .pos = {1, 1, -1},
            .normal = {1, 0, 0},
            .color = {0, 0, 0, 1},
            .uv = {1, 0},
        },
        {
            .pos = {1, 1, 1},
            .normal = {1, 0, 0},
            .color = {0, 0, 0, 1},
            .uv = {1, 1},
        },

        // side 4

        {
            .pos = {-1, -1, -1},
            .normal = {0, -1, 0},
            .color = {0, 0, 0, 1},
            .uv = {0, 0},
        },
        {
            .pos = {-1, -1, 1},
            .normal = {0, -1, 0},
            .color = {0, 0, 0, 1},
            .uv = {0, 1},
        },
        {
            .pos = {1, -1, -1},
            .normal = {0, -1, 0},
            .color = {0, 0, 0, 1},
            .uv = {1, 0},
        },
        {
            .pos = {1, -1, 1},
            .normal = {0, -1, 0},
            .color = {0, 0, 0, 1},
            .uv = {1, 1},
        },

        // side 5

        {
            .pos = {-1, 1, -1},
            .normal = {0, 1, 0},
            .color = {0, 0, 0, 1},
            .uv = {0, 0},
        },
        {
            .pos = {-1, 1, 1},
            .normal = {0, 1, 0},
            .color = {0, 0, 0, 1},
            .uv = {0, 1},
        },
        {
            .pos = {1, 1, -1},
            .normal = {0, 1, 0},
            .color = {0, 0, 0, 1},
            .uv = {1, 0},
        },
        {
            .pos = {1, 1, 1},
            .normal = {0, 1, 0},
            .color = {0, 0, 0, 1},
            .uv = {1, 1},
        },
};

static struct tri primitive_tris_cube[] = {
        {0, 1, 2},
        {2, 3, 1},
        {4, 5, 6},
        {6, 7, 5},
        {8, 9, 10},
        {10, 11, 9},
        {12, 13, 14},
        {14, 15, 13},
        {16, 17, 18},
        {18, 19, 17},
        {20, 21, 22},
        {22, 23, 21},
};

static float primitive_cube_idxs_floats[] = {
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
static int primitive_cube_idxs_ints[] = {
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

#endif //PRIMITIVES_H
