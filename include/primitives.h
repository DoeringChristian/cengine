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

#endif //PRIMITIVES_H
