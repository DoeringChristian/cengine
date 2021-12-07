#include "layer.h"

int layer_init_shader(struct layer *dst, int w, int h, const char *vert_path, const char *frag_path){
    layer_init(dst, w, h);
    layer_shader_load(dst, vert_path, frag_path);
    return 0;
}
int layer_init(struct layer *dst, int w, int h){
    dst->w = w;
    dst->h = h;
    dst->shader = (struct shader){
        .attr_idx = 0,
        .program = 0,
    };

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

    int idx = 0;
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct svert, pos);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct svert, uv);

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
    if(dst->shader.program != 0)
        layer_draw_shader(dst, &dst->shader);
    else
        return 1;
    return 0;
}
int layer_draw_shader(struct layer *dst, struct shader *shader){
    if(shader == NULL && dst->shader.program != 0)
        shader = &dst->shader;
    shader_bind(shader);

    texture_bind(&dst->texture, 0);
    shader_uniform_i(shader, "u_texture", 0);

    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_bind(&dst->ibo);

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

    glbuf_unbind(&dst->ibo);
    GLCall(glBindVertexArray(0));
    shader_unbind(shader);
    return 0;
}
int layer_blend(struct layer *dst, struct layer *src1, struct layer *src2, struct shader *bshader){
    shader_bind(bshader);

    layer_bind(dst);

    texture_bind(&src1->texture, 0);
    shader_uniform_i(bshader, "u_texture1", 0);

    texture_bind(&src1->texture, 1);
    shader_uniform_i(bshader, "u_texture2", 1);

    GLCall(glBindVertexArray(src1->gl_vao));

    glbuf_bind(&src1->ibo);

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

    glbuf_unbind(&src1->ibo);
    GLCall(glBindVertexArray(0));

    layer_unbind(dst);

    shader_unbind(bshader);
    return 0;
}
int layer_shader_load(struct layer *dst, const char *vert_path, const char *frag_path){
    shader_load(&dst->shader, vert_path, frag_path);
    return 0;
}
