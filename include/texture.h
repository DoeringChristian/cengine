#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"

struct texture{
    int w, h;
    int bpp;

    //GLuint gl_buf;
    GLuint gl_tex;

    // uniform name
    char *uname;
};

int texture_init_f32(struct texture *dst, int w, int h, float *src);
int texture_init_f32_uname(struct texture *dst, int w, int h, float *src, const char *name);
int texture_load(struct texture *dst, const char *path);
void texture_free(struct texture *dst);

int texture_resize_f32(struct texture *dst, int w, int h);

void texture_bind(struct texture *dst, GLuint slot);

int texture_set(struct texture *dst, int x, int y, float *src);

int textures_unbind();

#endif //TEXTURE_H
