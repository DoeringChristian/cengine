#ifndef TARGET_H
#define TARGET_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include "glbuf.h"
#include "texture.h"
#include "shader.h"

// screen space vertex
struct svert{
    float pos[2];
    float uv[2];
};

struct target{
    struct glbuf vbo;
    struct glbuf ibo;

    int w, h;

    GLuint gl_fbo;
    GLuint gl_vao;
    GLuint gl_rbo;

    struct texture texture;

    struct shader shader;
};

int target_init(struct target *dst, int w, int h, const char *path_vert, const char *path_frag);
void target_free(struct target *dst);

int target_bind(struct target *dst);
int target_unbind(struct target *dst);

int target_draw(struct target *dst);

#endif //TARGET_H
