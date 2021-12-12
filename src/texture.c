#include "texture.h"

int texture_init_f32_uname(struct texture *dst, int w, int h, float *src, const char *uname){
    texture_init_f32(dst, w, h, src);
    size_t uname_size = strlen(uname) + 1;
    dst->uname = malloc(uname_size);
    memcpy(dst->uname, uname, uname_size);
    dst->uname[uname_size-1] = 0;
    return 0;
}
int texture_init(struct texture *dst, int w, int h, float *src, GLenum internalformat){
    dst->w = w;
    dst->h = h;
    dst->bpp = 4;
    dst->uname = NULL;
    dst->type = GL_TEXTURE_2D;
    dst->internalformat = internalformat;

    size_t buf_size;
    float *buf = NULL;

    if(src == NULL){

        buf_size = sizeof(float) * dst->w * dst->h * dst->bpp;
        buf = malloc(buf_size);
        memset(buf, 0, buf_size);
    }

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(dst->type, dst->gl_tex));

    GLCall(glTexImage2D(dst->type, 0, dst->internalformat, dst->w, dst->h, 0, GL_RGBA, GL_FLOAT, src));

    //GLCall(glGenerateMipmap(dst->type));

    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(dst->type, 0));

    free(buf);
    return 0;
}
int texture_init_f32(struct texture *dst, int w, int h, float *src){
    return texture_init(dst, w, h, src, GL_RGBA32F);
}
int texture_init_rgbf16(struct texture *dst, int w, int h, float *src){
    return texture_init(dst, w, h, src, GL_RGBA16F);
}
int texture_init_rgbaf16(struct texture *dst, int w, int h, float *src){
    return texture_init(dst, w, h, src, GL_RGBA16F);
}
int texture_init_cube(struct texture *dst, int w, int h, float *src, GLenum internalformat){
    dst->w = w;
    dst->h = h;
    dst->bpp = 4;
    dst->uname = NULL;
    dst->type = GL_TEXTURE_CUBE_MAP;
    dst->internalformat = internalformat;

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, dst->gl_tex));

    for(size_t i = 0;i < 6;i++){
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, dst->internalformat, w, h, 0, GL_RGBA, GL_FLOAT, src));
    }

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

    return 0;
}
int texture_init_cube_rgbaf16(struct texture *dst, int w, int h, float *src){
    return texture_init_cube(dst, w, h, src, GL_RGBA16F);
}
int texture_init_cube_rgbf16(struct texture *dst, int w, int h, float *src){
    return texture_init_cube(dst, w, h, src, GL_RGB16F);
}
int texture_init_cube_f32(struct texture *dst, int w, int h, float *src){
    return texture_init_cube(dst, w, h, src, GL_RGBA32F);
}
int texture_init_depthcube(struct texture *dst, int w, int h, float *src, GLenum internalformat){
    dst->w = w;
    dst->h = h;
    dst->bpp = 4;
    dst->uname = NULL;
    dst->type = GL_TEXTURE_CUBE_MAP;
    dst->internalformat = internalformat;

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, dst->gl_tex));

    for(size_t i = 0;i < 6;i++){
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, dst->internalformat, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, src));
    }

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

    return 0;
}
int texture_init_depthcube_f16(struct texture *dst, int w, int h, float *src){
    return texture_init_depthcube(dst, w, h, src, GL_DEPTH_COMPONENT16);
}
int texture_init_depthcube_f32(struct texture *dst, int w, int h, float *src){
    return texture_init_depthcube(dst, w, h, src, GL_DEPTH_COMPONENT32);
#if 0
    dst->w = w;
    dst->h = h;
    dst->bpp = 4;
    dst->uname = NULL;
    dst->type = GL_TEXTURE_CUBE_MAP;

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, dst->gl_tex));

    for(size_t i = 0;i < 6;i++){
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, src));
    }

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

    return 0;
#endif
}

int texture_load(struct texture *dst, const char *path){
    dst->type = GL_TEXTURE_2D;
    dst->internalformat = GL_UNSIGNED_BYTE;
    stbi_set_flip_vertically_on_load(1);

    uint8_t *buf = stbi_load(path, &dst->w, &dst->h, &dst->bpp, 4);

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(dst->type, dst->gl_tex));

    GLCall(glTexImage2D(dst->type, 0, GL_RGBA8, dst->w, dst->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf));

    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(dst->type, 0));

    if(buf)
        stbi_image_free(buf);

    dst->uname = NULL;

    return 0;
}
int texture_load_hdr(struct texture *dst, const char *path){
    dst->type = GL_TEXTURE_2D;
    dst->internalformat = GL_RGB16F;
    dst->uname = NULL;
    stbi_set_flip_vertically_on_load(1);

    float *buf = stbi_loadf(path, &dst->w, &dst->h, &dst->bpp, 0);

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(GL_TEXTURE_2D, dst->gl_tex));
    GLCall(glTexImage2D(dst->type, 0, GL_RGB16F, dst->w, dst->h, 0, GL_RGB, GL_FLOAT, buf));

    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    stbi_image_free(buf);
    return 0;
}
int texture_resize_f32(struct texture *dst, int w, int h){
    if(w == dst->w && h == dst->h)
        return 0;
    dst->bpp = 4;

    size_t buf_old_size = sizeof(float) * dst->w * dst->h * dst->bpp;
    float *buf_old = malloc(buf_old_size);
    size_t buf_new_size = sizeof(float) * w * h * dst->bpp;
    float *buf_new = malloc(buf_new_size);
    memset(buf_old, 0, buf_old_size);

    GLCall(glBindTexture(dst->type, dst->gl_tex));
    GLCall(glGetTexImage(dst->type, 0, GL_RGBA, GL_FLOAT, buf_old));

    for(size_t i = 0;i < dst->w;i++){
        for(size_t j = 0;j < dst->h;j++){
            if(i < dst->w && j < dst->h){
                for(size_t k = 0;k < dst->bpp;k++){
                    buf_new[k + dst->bpp * (i + j*w)] = buf_old[k + dst->bpp * (i + j*dst->w)];
                }
            }
        }
    }

    // color is copied to false location
    //printf("%i %f\n", dst->w, buf_old[5]);

    GLCall(glBindTexture(dst->type, dst->gl_tex));
    GLCall(glTexImage2D(dst->type, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, buf_new));

    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(dst->type, 0));

    dst->w = w;
    dst->h = h;
    free(buf_old);
    free(buf_new);
    return 0;
}
void texture_free(struct texture *dst){
    GLCall(glDeleteTextures(1, &dst->gl_tex));
    if(dst->uname != NULL)
        free(dst->uname);
}

void texture_bind(struct texture *dst, GLuint slot){
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(dst->type, dst->gl_tex));
}
int texture_set(struct texture *dst, int x, int y, float *src){
    GLCall(glBindTexture(dst->type, dst->gl_tex));
    GLCall(glTexSubImage2D(dst->type, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, src));
    return 0;
}

int texture_unbind(struct texture *dst){
    GLCall(glBindTexture(dst->type, 0));
    return 0;
}
int texture_set_rect(struct texture *dst, int x, int y, int w, int h, float *src){
    GLCall(glBindTexture(GL_TEXTURE_2D, dst->gl_tex));
    GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_FLOAT, src));
    return 0;
}

int texture_fill(struct texture *dst, float *color){

    for(size_t i = 0;i < dst->w;i++){
        for(size_t j = 0;j < dst->h;j++){
            texture_set(dst, i, j, color);
        }
    }
    return 0;
}
int texture_load_hdr_cube(struct texture *dst, const char *path, int w, int h){
    dst->w = w;
    dst->h = h;
    struct texture tmp_tex;
    GLuint tmp_fbo, tmp_rbo;
    texture_load_hdr(&tmp_tex, path);

    // -------------------------------------------------
    // convert to cubemap texture.

    GLCall(glGenFramebuffers(1, &tmp_fbo));
    glGenRenderbuffers(1, &tmp_rbo);

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, tmp_fbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, tmp_fbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tmp_fbo));
    //GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dst->texture.gl_tex, 0));

    texture_init_cube_rgbf16(dst, w, h, NULL);

    texture_free(&tmp_tex);
}
