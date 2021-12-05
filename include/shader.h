#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_util.h"
#include <stdlib.h>
#include <stdio.h>

struct shader{
    GLuint program;
};

int shader_load(struct shader *dst, const char *vert_path, const char *frag_path);
void shader_free(struct shader *dst);

int shader_attach(struct shader *dst, const char *path, int shader_type);

int shader_uniform_i(struct shader *dst, const char *name, const int src);
int shader_uniform_f(struct shader *dst, const char *name, const float src);
int shader_uniform_mat4f(struct shader *dst, const char *name, const float *src);

#endif //SHADER_H
