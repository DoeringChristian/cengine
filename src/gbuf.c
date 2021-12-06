#include "gbuf.h"

int gbuf_init(struct gbuf *dst, void *src, GLsizei size, GLenum type, GLenum usage){
    dst->gl_type = type;
    dst->gl_usage = usage;
    dst->gl_buf_size = size;

    GLCall(glGenBuffers(1, &dst->gl_buf));
    GLCall(glBindBuffer(dst->gl_type, dst->gl_buf));

    if(src != NULL){
        GLCall(glBufferData(dst->gl_type, size, src, dst->gl_usage));
    }
    GLCall(glBindBuffer(dst->gl_type, 0));
    return 0;
}
void gbuf_free(struct gbuf *dst){
    GLCall(glBindBuffer(dst->gl_type, dst->gl_buf));
    GLCall(glDeleteBuffers(1, &dst->gl_buf));
    GLCall(glBindBuffer(dst->gl_type, 0));
    dst->gl_buf_size = 0;
}

int gbuf_set(struct gbuf *dst, void *src, GLsizei offset, GLsizei size){
    GLCall(glBindBuffer(dst->gl_type, dst->gl_buf));
    GLCall(glBufferSubData(dst->gl_type, offset, size, src));
    GLCall(glBindBuffer(dst->gl_type, 0));
    return 0;
}
int gbuf_get(struct gbuf *src, void *dst, GLsizei offset, GLsizei size){
    GLCall(glBindBuffer(src->gl_type, src->gl_buf));
    GLCall(glGetBufferSubData(src->gl_type, offset, size, dst));
    GLCall(glBindBuffer(src->gl_type, 0));
    return 0;
}

int gbuf_append(struct gbuf *dst, void *src, GLsizei size){
    uint8_t *buf = malloc(dst->gl_buf_size + size);
    memset(buf, 0, dst->gl_buf_size + size);

    gbuf_get(dst, buf, 0, dst->gl_buf_size);

    memcpy(&buf[dst->gl_buf_size], src, size);

    dst->gl_buf_size += size;

    GLCall(glBindBuffer(dst->gl_type, dst->gl_buf));
    GLCall(glBufferData(dst->gl_type, dst->gl_buf_size, buf, dst->gl_usage));
    GLCall(glBindBuffer(dst->gl_type, 0));
    free(buf);
    return 0;
}
int gbuf_insert(struct gbuf *dst, void *src, GLsizei size, size_t offset){
    uint8_t *buf = malloc(dst->gl_buf_size + size);
    memset(buf, 0, dst->gl_buf_size + size);

    size_t target = MIN(offset, dst->gl_buf_size);

    gbuf_get(dst, buf, 0, target);

    memcpy(&buf[target], src, size);

    gbuf_get(dst, &buf[target + size], target, dst->gl_buf_size - target);

    dst->gl_buf_size += size;

    GLCall(glBindBuffer(dst->gl_type, dst->gl_buf));
    GLCall(glBufferData(dst->gl_type, dst->gl_buf_size, buf, dst->gl_usage));
    GLCall(glBindBuffer(dst->gl_type, 0));
    free(buf);
    return 0;
}

int gbuf_resize(struct gbuf *dst, GLsizei size){
    void *buf = malloc(dst->gl_buf_size + size);
    memset(buf, 0, dst->gl_buf_size + size);

    gbuf_get(dst, buf, 0, dst->gl_buf_size);

    dst->gl_buf_size += size;

    GLCall(glBindBuffer(dst->gl_type, dst->gl_buf));
    GLCall(glBufferData(dst->gl_type, dst->gl_buf_size, buf, dst->gl_usage));
    GLCall(glBindBuffer(dst->gl_type, 0));
    free(buf);
    return 0;
}
void gbuf_bind(struct gbuf *dst){
    GLCall(glBindBuffer(dst->gl_type, dst->gl_buf));
}
void gbuf_unbind(struct gbuf *dst){
    GLCall(glBindBuffer(dst->gl_type, 0));
}
size_t gbuf_size(struct gbuf *src){
    return src->gl_buf_size;
}
