#include "gbuf.h"

int gbuf_init_shader(struct gbuf *dst, int w, int h, const char *vert_path, const char *frag_path){
    gbuf_init(dst, w, h);
    gbuf_shader_load(dst, vert_path, frag_path);
    return 0;
}
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

    textures_unbind();

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

int gbuf_draw(struct gbuf *dst, struct lvert light){
    if(dst->shader.program == 0)
        return 1;
    gbuf_draw_shader(dst, &dst->shader, light);
    return 0;
}
int gbuf_draw_shader(struct gbuf *dst, struct shader *shader, struct lvert light){
    if(shader == NULL && dst->shader.program != 0)
        shader = &dst->shader;
    shader_bind(shader);

    texture_bind(&dst->pos, 0);
    shader_uniform_i(shader, "u_pos", 0);

    texture_bind(&dst->normal, 1);
    shader_uniform_i(shader, "u_noraml", 1);

    texture_bind(&dst->color, 2);
    shader_uniform_i(shader, "u_color", 2);

    shader_uniform_vec4f(shader, "u_light_pos", light.pos);
    shader_uniform_vec4f(shader, "u_light_color", light.color);

    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_bind(&dst->ibo);

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

    textures_unbind();
    glbuf_unbind(&dst->ibo);
    GLCall(glBindVertexArray(0));
    shader_unbind(shader);
    return 0;
}
int gbuf_shader_load(struct gbuf *dst, const char *vert_path, const char *frag_path){
    shader_load(&dst->shader, vert_path, frag_path);
    return 0;
}
