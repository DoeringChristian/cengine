#include "cubelayer.h"

int cubelayer_init_depthcube(struct cubelayer *dst, int w, int h){
    dst->w = w;
    dst->h = h;

    // texture
    texture_init_depthcube_f16(&dst->texture, w, h, NULL);
    // /texture

    GLCall(glGenFramebuffers(1, &dst->gl_fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    //GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dst->texture.gl_tex, 0));
    GLCall(glDrawBuffer(GL_NONE));
    GLCall(glReadBuffer(GL_NONE));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    



    //output side

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
    return 0;
}
int cubelayer_free(struct cubelayer *dst){
    texture_free(&dst->texture);
    GLCall(glDeleteFramebuffers(1, &dst->gl_fbo));
    return 0;
}

int cubelayer_bind(struct cubelayer *dst, int face){
    GLCall(glViewport(0, 0, dst->w, dst->h));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, dst->texture.gl_tex, 0));
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
    GLCall(glEnable(GL_DEPTH_TEST));
    return 0;
}
int cubelayer_unbind(struct cubelayer *dst){
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    return 0;
}
int cubelayer_draw(struct cubelayer *dst, struct shader *shader){
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
