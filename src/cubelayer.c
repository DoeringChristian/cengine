#include "cubelayer.h"

int cubelayer_init(struct cubelayer *dst, int w, int h){
    dst->w = w;
    dst->h = h;
    GLCall(glGenFramebuffers(1, &dst->gl_fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));

    GLCall(glGenTextures(1, &dst->texture));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, dst->texture));

#if 1
    for(size_t i = 0;i < 6;i++){
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    }
#else
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
#endif

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dst->texture, 0));
    GLCall(glDrawBuffer(GL_NONE));
    GLCall(glReadBuffer(GL_NONE));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    return 0;
}
int cubelayer_free(struct cubelayer *dst){
    GLCall(glDeleteTextures(1, &dst->texture));
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
