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

    GLuint gl_buf;
    GLuint gl_tex;
};

int texture_load(struct texture *dst, const char *path);
void texture_free(struct texture *dst);

void texture_bind(struct texture *dst, GLuint slot);

#endif //TEXTURE_H
