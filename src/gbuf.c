#include "gbuf.h"

int gbuf_init(struct gbuf *dst, int w, int h, const char *path_vert, const char *path_frag){
    dst->w = w;
    dst->h = h;

    shader_load(&dst->shader, path_vert, path_frag);

    // input side

    GLCall(glGenFramebuffers(1, &dst->gl_fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));

    texture_init_f32(&dst->pos, w, h, NULL);
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst->pos.gl_tex, 0));

    texture_init_f32(&dst->normal, w, h, NULL);
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst->normal.gl_tex, 0));

    texture_init_f32(&dst->color, w, h, NULL);
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst->pos.gl_tex, 0));

    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dst->gl_rbo));

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return -1;
    
    GLuint attachments[3] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
    };

    GLCall(glDrawBuffers(3, attachments));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // output siede

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
void gbuf_free(struct gbuf *dst){
    texture_free(&dst->pos);
    texture_free(&dst->normal);
    texture_free(&dst->color);
    GLCall(glDeleteFramebuffers(1, &dst->gl_fbo));
    GLCall(glDeleteRenderbuffers(1, &dst->gl_rbo));
    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
    glbuf_free(&dst->vbo);
    glbuf_free(&dst->ibo);
    shader_free(&dst->shader);
}

int gbuf_bind(struct gbuf *dst){
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glClearColor(0, 0, 0, 1));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glEnable(GL_DEPTH_TEST));

    return 0;
}
int gbuf_unbind(struct gbuf *dst){
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glClearColor(0, 0, 0, 1));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    return 0;
}

int gbuf_draw_debug(struct gbuf *dst){
    shader_bind(&dst->shader);

    texture_bind(&dst->pos, 0);
    shader_uniform_i(&dst->shader, "u_pos", 0);

    texture_bind(&dst->normal, 1);
    shader_uniform_i(&dst->shader, "u_noraml", 1);

    texture_bind(&dst->pos, 2);
    shader_uniform_i(&dst->shader, "u_color", 2);

    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_bind(&dst->ibo);

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

    glbuf_unbind(&dst->ibo);
    GLCall(glBindVertexArray(0));
    shader_unbind(&dst->shader);
    return 0;
}
