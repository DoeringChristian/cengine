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
int mesh_init_cube(struct mesh *dst, struct shader *shader){
    struct vert verts[] = {
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

    struct tri tris[] = {
        {0, 1, 2},
        {2, 3, 0},
        {3, 4, 5},
        {4, 6, 3},
        {6, 7, 8},
        {2, 9, 6},
    };
    mesh_init(dst, shader, verts, 24, tris, 6);

}
