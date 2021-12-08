#include "primitives.h"

int mesh_init_quad(struct mesh *dst){

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

    mesh_init(dst, verts, 4, tris, 2);

    return 0;
}
int mesh_init_cube(struct mesh *dst){
#if 1
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
#endif
    struct cmesh tmp;
    cmesh_init_cube(&tmp);
    
    mesh_init(dst, verts, 24, tris, 12);
    //mesh_init_cmesh(dst, &tmp);

    cmesh_free(&tmp);
    return 0;
}
int cmesh_init_quad(struct cmesh *dst){
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

    cmesh_init(dst);
    cmesh_vert_append(dst, verts, 4);
    cmesh_tri_append(dst, tris, 2);
    return 0;
}
int cmesh_init_cube(struct cmesh *dst){
    cmesh_init(dst);

    struct cmesh tmp;
    cmesh_init_quad(&tmp);
    cmesh_set_pos(&tmp, (float []){0, 0, -1});
    cmesh_set_rot_axis(&tmp, (float []){1, 0, 0}, M_PI);
    cmesh_append(dst, &tmp);
    cmesh_free(&tmp);

    cmesh_init_quad(&tmp);
    cmesh_set_pos(&tmp, (float []){0, 0, 1});
    cmesh_set_rot_axis(&tmp, (float []){1, 0, 0}, 0);
    cmesh_append(dst, &tmp);
    cmesh_free(&tmp);

    cmesh_init_quad(&tmp);
    cmesh_set_pos(&tmp, (float []){-1, 0, 0});
    cmesh_set_rot_axis(&tmp, (float []){0, 1, 0}, M_PI);
    cmesh_append(dst, &tmp);
    cmesh_free(&tmp);

    cmesh_init_quad(&tmp);
    cmesh_set_pos(&tmp, (float []){1, 0, 0});
    cmesh_set_rot_axis(&tmp, (float []){0, 1, 0}, 0);
    cmesh_append(dst, &tmp);
    cmesh_free(&tmp);

    cmesh_init_quad(&tmp);
    cmesh_set_pos(&tmp, (float []){0, -1, 0});
    cmesh_set_rot_axis(&tmp, (float []){1, 0, 0}, M_PI);
    cmesh_append(dst, &tmp);
    cmesh_free(&tmp);

    cmesh_init_quad(&tmp);
    cmesh_set_pos(&tmp, (float []){0, 1, 0});
    cmesh_set_rot_axis(&tmp, (float []){1, 0, 0}, 0);
    cmesh_append(dst, &tmp);
    cmesh_free(&tmp);
}

