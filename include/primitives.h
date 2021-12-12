#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "mesh.h"
#include "cmesh.h"
#include "shader.h"
#include "gl_util.h"

static int primitives_initialized = 0;

extern struct mesh2 mesh2_quad;
extern struct mesh3 mesh3_cube;

int mesh_init_quad(struct mesh *dst);
int mesh_init_cube(struct mesh *dst);

int cmesh_init_quad(struct cmesh *dst);
int cmesh_init_cube(struct cmesh *dst);

int primitives_init();
void primitives_free();

static struct vert vert_quad[] = {
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

static struct tri tri_quad[] = {
    {
        0, 1, 2,
    },
    {
        2, 3, 1,
    },
};

static struct vert vert_cube[] = {
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

static struct tri tri_cube[] = {
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

extern const char shader_vert_quad_src[];
extern const char shader_vert_cm_src[];
extern const char shader_vert_src[];
extern const char shader_vert_shadow_src[];

extern const char shader_frag_src[];
extern const char shader_frag_forward_src[];
extern const char shader_frag_shadow_src[];
extern const char shader_frag_lighting_src[];
extern const char shader_frag_emission_src[];
extern const char shader_frag_clib_src[];
extern const char shader_frag_blurh_src[];
extern const char shader_frag_blurv_src[];
extern const char shader_frag_gamma_correct_src[];
extern const char shader_frag_skybox_src[];
extern const char shader_frag_ambient_src[];
extern const char shader_frag_eqr_to_cm_src[];
extern const char shader_frag_irr_src[];
extern const char shader_frag_ref_src[];
extern const char shader_frag_brdf_int_src[];



#if 0
static const char shader_vert_quad_src[] = {
#include "vert_quad.glsl"
};
static const char shader_vert_cm_src[] = {
#include "vert_cm.glsl"
};
static const char shader_frag_forward_src[] = {
#include "frag_forward.glsl"
};
static const char shader_frag_blurh_src[] = {
#include "frag_blurh.glsl"
};
static const char shader_frag_blurv_src[] = {
#include "frag_blurv.glsl"
};
static const char shader_frag_clib_src[] = {
#include "frag_clip.glsl"
};
#endif


#endif //PRIMITIVES_H
