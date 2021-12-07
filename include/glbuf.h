#ifndef GBUF_H
#define GBUF_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <gl_util.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

struct glbuf{
    GLuint gl_buf;
    GLsizei gl_buf_size;
    GLenum gl_type;
    GLenum gl_usage;
};

int glbuf_init(struct glbuf *dst, void *src, GLsizei size, GLenum type, GLenum usage);
void glbuf_free(struct glbuf *dst);

int glbuf_set(struct glbuf *dst, void *src, GLsizei offset, GLsizei size);
int glbuf_get(struct glbuf *src, void *dst, GLsizei offset, GLsizei size);

int glbuf_append(struct glbuf *dst, void *src, GLsizei size);
int glbuf_insert(struct glbuf *dst, void *src, GLsizei size, size_t offset);

int glbuf_resize(struct glbuf *dst, GLsizei size);

void glbuf_bind(struct glbuf *dst);
void glbuf_unbind(struct glbuf *dst);

size_t glbuf_size(struct glbuf *src);


#endif //GBUF_H
