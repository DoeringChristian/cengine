#ifndef GBUF_H
#define GBUF_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <gl_util.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

struct gbuf{
    GLuint gl_buf;
    GLsizei gl_buf_size;
    GLenum gl_type;
    GLenum gl_usage;
};

int gbuf_init(struct gbuf *dst, void *src, GLsizei size, GLenum type, GLenum usage);
void gbuf_free(struct gbuf *dst);

int gbuf_set(struct gbuf *dst, void *src, GLsizei offset, GLsizei size);
int gbuf_get(struct gbuf *src, void *dst, GLsizei offset, GLsizei size);

int gbuf_append(struct gbuf *dst, void *src, GLsizei size);
int gbuf_insert(struct gbuf *dst, void *src, GLsizei size, size_t offset);

int gbuf_resize(struct gbuf *dst, GLsizei size);

void gbuf_bind(struct gbuf *dst);
void gbuf_unbind(struct gbuf *dst);

size_t gbuf_size(struct gbuf *src);


#endif //GBUF_H
