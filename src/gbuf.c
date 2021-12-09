#include "gbuf.h"

int gbuf_init(struct gbuf *dst, int w, int h){
    dst->w = w;
    dst->h = h;

    // input side

    GLCall(glGenFramebuffers(1, &dst->gl_fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));

    texture_init_f32(&dst->pos, w, h, NULL);
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst->pos.gl_tex, 0));

    texture_init_f32(&dst->normal, w, h, NULL);
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, dst->normal.gl_tex, 0));

    texture_init_f32(&dst->color, w, h, NULL);
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, dst->color.gl_tex, 0));

    texture_unbind(&dst->color);

    GLCall(glGenRenderbuffers(1, &dst->gl_rbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

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

    // output side

    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_init(&dst->vbo, svert_quad, sizeof(svert_quad), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    glbuf_init(&dst->ibo, idxs_quad, sizeof(idxs_quad), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_bind(&dst->vbo);

    int idx = 0;
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct svert, pos);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct svert, uv);

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

int gbuf_draw_ps(struct gbuf *src, struct shader *shader, struct texture *shadow_depth, struct texture *light_prev, struct light *light, struct cvert *camera){
    if(shader == NULL)
        return -1;

    shader_bind(shader);

    // set textures
    texture_bind(&src->pos, 0);
    shader_uniform_i(shader, "u_pos", 0);

    texture_bind(&src->normal, 1);
    shader_uniform_i(shader, "u_normal", 1);

    texture_bind(&src->color, 2);
    shader_uniform_i(shader, "u_color", 2);

    texture_bind(shadow_depth, 3);
    shader_uniform_i(shader, "u_shadow_depth", 3);

    texture_bind(light_prev, 4);
    shader_uniform_i(shader, "u_light_prev", 4);

    // set light parameters
    shader_uniform_vec4f(shader, "u_light_pos", light->pos);
    shader_uniform_vec4f(shader, "u_light_color", light->color);
    shader_uniform_f(shader, "u_light_c1", light->c1);
    shader_uniform_f(shader, "u_light_c2", light->c2);

    shader_uniform_f(shader, "u_shadow_len", light->shadow_len);

    shader_uniform_vec4f(shader, "u_view_pos", camera->view[3]);

    GLCall(glBindVertexArray(src->gl_vao));

    glbuf_bind(&src->ibo);

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

    glbuf_unbind(&src->ibo);
    GLCall(glBindVertexArray(0));
    shader_unbind(shader);
    return 0;
}
