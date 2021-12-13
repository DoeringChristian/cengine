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
    int mmlvl;

    //GLuint gl_buf;
    GLuint gl_tex;
    GLenum type;
    GLenum internalformat;

    // uniform name
    char *uname;
};

int texture_init(struct texture *dst, int w, int h, float *src, GLenum internalformat, GLsizei mmlvl);
int texture_init_rgbf16(struct texture *dst, int w, int h, float *src);
int texture_init_rgbaf16(struct texture *dst, int w, int h, float *src);
int texture_init_rgbaf16_m(struct texture *dst, int w, int h, float *src, GLsizei mmlvl);
int texture_init_f32(struct texture *dst, int w, int h, float *src);
int texture_init_f32_uname(struct texture *dst, int w, int h, float *src, const char *name);
int texture_init_cube(struct texture *dst, int w, int h, float *src, GLenum internalformat, GLsizei mmlvl);
int texture_init_cube_rgbf16(struct texture *dst, int w, int h, float *src);
int texture_init_cube_rgbaf16(struct texture *dst, int w, int h, float *src);
int texture_init_cube_f32(struct texture *dst, int w, int h, float *src);
int texture_init_depthcube(struct texture *dst, int w, int h, float *src, GLenum internalformat, GLsizei mmlvl);
int texture_init_depthcube_f16(struct texture *dst, int w, int h, float *src);
int texture_init_depthcube_f32(struct texture *dst, int w, int h, float *src);
int texture_load(struct texture *dst, const char *path);
int texture_load_hdr(struct texture *dst, const char *path);
int texture_load_hdr_cube(struct texture *dst, const char *path, int w, int h);
void texture_free(struct texture *dst);

int texture_resize_f32(struct texture *dst, int w, int h);

void texture_bind(struct texture *dst, GLuint slot);

int texture_set(struct texture *dst, int x, int y, float *src);
int texture_set_rect(struct texture *dst, int x, int y, int w, int h, float *src);

int texture_fill(struct texture *dst, float *color);

int texture_unbind(struct texture *dst);

#endif //TEXTURE_H
