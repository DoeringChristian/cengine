#include "layer.h"

int layer_init(struct layer *dst, int w, int h){
    return layer_init_n(dst, w, h, 1);
}
int layer_init_n(struct layer *dst, int w, int h, int num_textures){
    if(num_textures > 16)
        return -1;
    dst->w = w;
    dst->h = h;
    darray_init(&dst->textures, num_textures);

    // input side
    
    GLCall(glGenFramebuffers(1, &dst->gl_fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));

    // push initial texture
    struct texture texture;
    for(size_t i = 0;i < num_textures;i++){
        texture_init_f32(&texture, w, h, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture.gl_tex, 0);
        darray_push_back(&dst->textures, texture);
    }

    GLCall(glGenRenderbuffers(1, &dst->gl_rbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dst->gl_rbo));

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return -1;

    GLuint attachments[num_textures];

    for(size_t i = 0;i < num_textures;i++){
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glDrawBuffers(num_textures, attachments);

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // output side

    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_init(&dst->vbo, svert_quad, sizeof(svert_quad), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    glbuf_init(&dst->ibo, idxs_quad, sizeof(idxs_quad), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    glbuf_bind(&dst->vbo);

    int idx = 0;
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct svert, pos);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct svert, uv);

    glbuf_unbind(&dst->vbo);

    GLCall(glBindVertexArray(0));

    return 0;
}
void layer_free(struct layer *dst){
    for(size_t i = 0;i < darray_len(&dst->textures);i++)
        texture_free(&dst->textures[i]);
    darray_free(&dst->textures);
    GLCall(glDeleteFramebuffers(1, &dst->gl_fbo));
    GLCall(glDeleteRenderbuffers(1, &dst->gl_rbo));
    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
    glbuf_free(&dst->vbo);
    glbuf_free(&dst->ibo);
}
int layer_bind(struct layer *dst){
    GLCall(glViewport(0, 0, dst->w, dst->h));
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
int layer_draw(struct layer *dst, struct shader *shader){
    if(shader == NULL)
        return -1;

    shader_bind(shader);

    texture_bind(&dst->textures[0], 0);
    shader_uniform_i(shader, "u_texture", 0);

    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_bind(&dst->ibo);

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

    glbuf_unbind(&dst->ibo);
    GLCall(glBindVertexArray(0));
    shader_unbind(shader);
    return 0;
}
int layer_draw_tex(struct layer *dst, struct shader *shader, struct texture *tex){
    if(shader == NULL)
        return -1;

    shader_bind(shader);

    texture_bind(&dst->textures[0], 0);
    shader_uniform_i(shader, "u_texture1", 0);

    texture_bind(tex, 1);
    shader_uniform_i(shader, "u_texture2", 1);

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

    texture_bind(&src1->textures[0], 0);
    shader_uniform_i(bshader, "u_texture1", 0);

    texture_bind(&src2->textures[0], 1);
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
int layer_clear(struct layer *dst){
    float color_black[4] = {
        0, 0, 0, 0,
    };
    for(size_t i = 0;i < darray_len(&dst->textures);i++)
        texture_fill(&dst->textures[0], color_black);

    return 0;
}
