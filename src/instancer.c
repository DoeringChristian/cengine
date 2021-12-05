#include "instancer.h"

int instancer_init(struct instancer *dst, struct attr_arr *attr_arr){
    darray_init(&dst->instances, 2);

    GLCall(glGenBuffers(1, &dst->gl_vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));

    instancer_attr_push(dst, 16, GL_FLOAT, 0);
    instancer_attr_push(dst, 1, GL_FLOAT, 0);

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

    dst->gl_vbo_size = 0;

    return 0;
}
void instancer_free(struct instancer *dst){
    darray_free(&dst->instances);
    GLCall(glDeleteBuffers(1, &dst->gl_vbo));
}

int instancer_resize(struct instancer *dst){
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, darray_size(&dst->instances), dst->instances, GL_DYNAMIC_DRAW));

    dst->gl_vbo_size = darray_size(&dst->instances);

    return 0;
}
int instancer_update(struct instancer *dst){
    if(dst->gl_vbo_size != darray_size(&dst->instances))
        instancer_resize(dst);

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, dst->gl_vbo_size, dst->instances));

    return 0;
}

int instancer_push(struct instancer *dst, struct instance src){
    return darray_push_back(&dst->instances, src);
}

int instancer_attr_push(struct instancer *dst, GLsizei num, GLenum type, GLint normalized){
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));
    attr_arr_push(dst->attr_arr, num, type, normalized);
    GLCall(glVertexAttribDivisor(dst->attr_arr->idx, 1));
    return 0;
}
