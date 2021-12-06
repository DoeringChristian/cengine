#include "texture.h"

int texture_init(struct texture *dst, int w, int h, float *src){
    dst->w = w;
    dst->h = h;
    dst->bpp = 4;

    //size_t buf_size = sizeof(float) * dst->w * dst->h * dst->bpp;
    //uint8_t *buf = malloc(buf_size);
    //memset(buf, 0, buf_size);

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(GL_TEXTURE_2D, dst->gl_tex));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, dst->w, dst->h, 0, GL_RGBA, GL_FLOAT, src));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    //free(buf);
    return 0;
}

int texture_load(struct texture *dst, const char *path){
    stbi_set_flip_vertically_on_load(1);

    uint8_t *buf = stbi_load(path, &dst->w, &dst->h, &dst->bpp, 4);

    GLCall(glGenTextures(1, &dst->gl_tex));
    GLCall(glBindTexture(GL_TEXTURE_2D, dst->gl_tex));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, dst->w, dst->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if(buf)
        stbi_image_free(buf);

    return 0;
}
int texture_resize(struct texture *dst, int w, int h){
    if(w == dst->w && h == dst->h)
        return 0;
    dst->w = w;
    dst->h = h;
    dst->bpp = 4;

    size_t buf_size = sizeof(float) * dst->w * dst->h * dst->bpp;
    float *buf = malloc(buf_size);
    memset(buf, 0, w * h * dst->bpp);

    GLCall(glBindTexture(GL_TEXTURE_2D, dst->gl_tex));
    GLCall(glGetnTexImage(GL_TEXTURE_2D, 0, GL_RGBA8, GL_FLOAT, buf_size, buf));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, dst->w, dst->h, 0, GL_RGBA, GL_FLOAT, buf));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    free(buf);
    return 0;
}
void texture_free(struct texture *dst){
    GLCall(glDeleteTextures(1, &dst->gl_tex));
}

void texture_bind(struct texture *dst, GLuint slot){
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    glBindTexture(GL_TEXTURE_2D, dst->gl_tex);
}
int texture_set(struct texture *dst, int x, int y, float *src){
    GLCall(glBindTexture(GL_TEXTURE_2D, dst->gl_tex));
    GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, src));
    return 0;
}

