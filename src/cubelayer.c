#include "cubelayer.h"

int cubelayer_init(struct cubelayer *dst, int w, int h){
    dst->w = w;
    dst->h = h;

    // texture
    texture_init_f32_depthcube(&dst->texture, w, h, NULL);
    // /texture

    GLCall(glGenFramebuffers(1, &dst->gl_fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dst->texture.gl_tex, 0));
    GLCall(glDrawBuffer(GL_NONE));
    GLCall(glReadBuffer(GL_NONE));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    return 0;
}
int cubelayer_free(struct cubelayer *dst){
    texture_free(&dst->texture);
    GLCall(glDeleteFramebuffers(1, &dst->gl_fbo));
    return 0;
}

int cubelayer_bind(struct cubelayer *dst){
    GLCall(glViewport(0, 0, dst->w, dst->h));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
    return 0;
}
int cubelayer_unbind(struct cubelayer *dst){
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    return 0;
}
