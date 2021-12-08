#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include <stdlib.h>
#include <stdio.h>

#define shader_attr_push_s(_dst_p, _type, _normalized, _struct, _element){\
    _struct _tmp;\
    shader_attr_push(_dst_p, sizeof(_tmp._element)/attr_size(_type), _type, _normalized, sizeof(_tmp), (void *)offsetof(_struct, _element));\
}

#define shader_attr_push_mat4f_s(_dst_p, _normalized, _struct, _element){\
    shader_attr_push_mat4f(_dst_p, _normalized, sizeof(_struct), (void *)offsetof(_struct, _element));\
}

#define shader_attr_push_mat4f_div_s(_dst_p, _normalized, _struct, _element){\
    shader_attr_push_mat4f_div(_dst_p, _normalized, sizeof(_struct), (void *)offsetof(_struct, _element));\
}

struct shader{
    GLuint program;
    size_t attr_idx;
};

int shader_load(struct shader *dst, const char *vert_path, const char *frag_path);
int shader_load_vgf(struct shader *dst, const char *vert_path, const char *geo_path, const char *frag_path);
void shader_free(struct shader *dst);

int shader_attach(struct shader *dst, const char *path, int shader_type);

int shader_uniform_i(struct shader *dst, const char *name, const int src);
int shader_uniform_f(struct shader *dst, const char *name, const float src);
int shader_uniform_mat4f(struct shader *dst, const char *name, const float *src);
int shader_uniform_vec4f(struct shader *dst, const char *name, const float *src);

int shader_attr_push(struct shader *dst, GLsizei num, GLenum type, GLboolean normalized, GLsizei stride, const void *offset);
int shader_attr_push_mat4f(struct shader *dst, GLboolean normalized, GLsizei stride, const void *offset);
int shader_attr_push_mat4f_div(struct shader *dst, GLboolean normalized, GLsizei stride, const void *offset);

int shader_bind(struct shader *src);
int shader_unbind(struct shader *src);

#endif //SHADER_H
