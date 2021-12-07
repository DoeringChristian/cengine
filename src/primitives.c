#include "primitives.h"

int mesh_init_quad(struct mesh *dst, struct shader *shader){

    struct vert verts[4] = {
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

    struct tri tris[2] = {
        {
            0, 1, 2,
        },
        {
            2, 3, 1,
        },
    };

    mesh_init(dst, shader, verts, 4, tris, 2);

    return 0;
}
