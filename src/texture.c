#include "texture.h"

int texture_init_f32_uname(struct texture *dst, int w, int h, float *src, const char *uname){
    texture_init_f32(dst, w, h, src);
    size_t uname_size = strlen(uname) + 1;
    dst->uname = malloc(uname_size);
    memcpy(dst->uname, uname, uname_size);
    dst->uname[uname_size-1] = 0;
    return 0;
}
int texture_init_f32(struct texture *dst, int w, int h, float *src){
    dst->s = w;
    dst->t = h;
    dst->r = 0;
    dst->bpp = 4;
    dst->uname = NULL;
    dst->type = GL_TEXTURE_2D;

    size_t buf_size;
    float *buf = NULL;

    if(src == NULL){

        buf_size = sizeof(float) * dst->s * dst->t * dst->bpp;
        buf = malloc(buf_size);
        memset(buf, 0, buf_size);
    }

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(dst->type, dst->gl_tex));

    GLCall(glTexImage2D(dst->type, 0, GL_RGBA32F, dst->s, dst->t, 0, GL_RGBA, GL_FLOAT, src));

    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(dst->type, 0));

    free(buf);
    return 0;
}
int texture_init_f32_depthcube(struct texture *dst, int s, int t, int r, float *src){
    dst->s = s;
    dst->t = t;
    dst->r = r;
    dst->bpp = 4;
    dst->uname = NULL;
    dst->type = GL_TEXTURE_CUBE_MAP;

    size_t buf_size;
    float *buf = NULL;

#if 0
    if(src == NULL){

        buf_size = sizeof(float) * dst->bpp * (2*x*y + 2*y*z + 2*x*z);
        buf = malloc(buf_size);
        memset(buf, 0, buf_size);
    }
#endif

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(dst->type, dst->gl_tex));

    //GLCall(glTexImage2D(dst->type, 0, GL_RGBA32F, dst->x, dst->y, 0, GL_RGBA, GL_FLOAT, src));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT, dst->t, dst->r, 0, GL_DEPTH_COMPONENT, GL_FLOAT, src));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_DEPTH_COMPONENT, dst->t, dst->r, 0, GL_DEPTH_COMPONENT, GL_FLOAT, src));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_DEPTH_COMPONENT, dst->s, dst->r, 0, GL_DEPTH_COMPONENT, GL_FLOAT, src));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_DEPTH_COMPONENT, dst->s, dst->r, 0, GL_DEPTH_COMPONENT, GL_FLOAT, src));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_DEPTH_COMPONENT, dst->s, dst->t, 0, GL_DEPTH_COMPONENT, GL_FLOAT, src));
    GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_DEPTH_COMPONENT, dst->s, dst->t, 0, GL_DEPTH_COMPONENT, GL_FLOAT, src));

    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(dst->type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(dst->type, 0));

    free(buf);
    return 0;
}

int texture_load(struct texture *dst, const char *path){
    dst->type = GL_TEXTURE_2D;
    dst->r = 0;
    stbi_set_flip_vertically_on_load(1);

    uint8_t *buf = stbi_load(path, &dst->s, &dst->t, &dst->bpp, 4);

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(dst->type, dst->gl_tex));

    GLCall(glTexImage2D(dst->type, 0, GL_RGBA8, dst->s, dst->t, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf));

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
int texture_resize_f32(struct texture *dst, int w, int h){
    if(w == dst->s && h == dst->t)
        return 0;
    dst->bpp = 4;

    size_t buf_old_size = sizeof(float) * dst->s * dst->t * dst->bpp;
    float *buf_old = malloc(buf_old_size);
    size_t buf_new_size = sizeof(float) * w * h * dst->bpp;
    float *buf_new = malloc(buf_new_size);
    memset(buf_old, 0, buf_old_size);

    GLCall(glBindTexture(dst->type, dst->gl_tex));
    GLCall(glGetTexImage(dst->type, 0, GL_RGBA, GL_FLOAT, buf_old));

    for(size_t i = 0;i < dst->s;i++){
        for(size_t j = 0;j < dst->t;j++){
            if(i < dst->s && j < dst->t){
                for(size_t k = 0;k < dst->bpp;k++){
                    buf_new[k + dst->bpp * (i + j*w)] = buf_old[k + dst->bpp * (i + j*dst->s)];
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

    dst->s = w;
    dst->t = h;
    free(buf_old);
    free(buf_new);
    return 0;
}
void texture_free(struct texture *dst){
    GLCall(glDeleteTextures(1, &dst->gl_tex));
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

    for(size_t i = 0;i < dst->s;i++){
        for(size_t j = 0;j < dst->t;j++){
            texture_set(dst, i, j, color);
        }
    }
    return 0;
}
