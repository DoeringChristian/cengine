#include "layer.h"

int layer_init(struct layer *dst, int w, int h, const char *vert_path, const char *frag_path){
    dst->w = w;
    dst->h = h;
    shader_load(&dst->shader, vert_path, frag_path);

    // input side
    
    GLCall(glGenFramebuffers(1, &dst->gl_fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));

    texture_init_f32(&dst->texture, w, h, NULL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst->texture.gl_tex, 0);

    GLCall(glGenRenderbuffers(1, &dst->gl_rbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dst->gl_rbo));

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return -1;

    GLuint attachments[1] = {
        GL_COLOR_ATTACHMENT0,
    };

    glDrawBuffers(1, attachments);

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // output side

    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_init(&dst->vbo, svert_quad, sizeof(svert_quad), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    glbuf_init(&dst->ibo, idxs_quad, sizeof(idxs_quad), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_bind(&dst->vbo);

    shader_attr_push_s(&dst->shader, GL_FLOAT, 0, struct svert, pos);
    shader_attr_push_s(&dst->shader, GL_FLOAT, 0, struct svert, uv);

    glbuf_unbind(&dst->vbo);

    GLCall(glBindVertexArray(0));

    return 0;
}
void layer_free(struct layer *dst){
    texture_free(&dst->texture);
    GLCall(glDeleteFramebuffers(1, &dst->gl_fbo));
    GLCall(glDeleteRenderbuffers(1, &dst->gl_rbo));
    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
    glbuf_free(&dst->vbo);
    glbuf_free(&dst->ibo);
    shader_free(&dst->shader);
}
int layer_bind(struct layer *dst){
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glClearColor(0, 0, 0, 1));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glEnable(GL_DEPTH_TEST));

    return 0;
}
int layer_unbind(struct layer *dst){
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glClearColor(0, 0, 0, 1));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    return 0;
}
int layer_draw(struct layer *dst){
    shader_bind(&dst->shader);

    texture_bind(&dst->texture, 0);
    shader_uniform_i(&dst->shader, "u_texture", 0);

    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_bind(&dst->ibo);

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

    glbuf_unbind(&dst->ibo);
    GLCall(glBindVertexArray(0));
    shader_unbind(&dst->shader);
    return 0;
}
