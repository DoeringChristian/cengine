#include "shader.h"

int _shader_load(GLuint *dst, const char *path, int shader_type){
    FILE *fp = fopen(path, "r");
    if(fp == NULL){
        printf("[Error opening shader] type: %i\n path: %s\n", shader_type, path);
        return 0;
    }

    size_t src_size;
    char *src;
    int status;

    fseek(fp, 0, SEEK_END);
    src_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    src = malloc(sizeof(char) * (src_size + 1));

    fread(src, 1, src_size, fp);

    src[src_size] = 0;

    const char *tmp = src;

    *dst = glCreateShader(shader_type);
    GLCall(glShaderSource(*dst, 1, &tmp, (int *)&src_size));
    GLCall(glCompileShader(*dst));

    glGetShaderiv(*dst, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE){
        int log_len;
        glGetShaderiv(*dst, GL_INFO_LOG_LENGTH, &log_len);

        char buf[log_len + 1];
        glGetShaderInfoLog(*dst, log_len, NULL, buf);

        printf("[Error compiling shader] type: %i, log: %spath: %s\n", shader_type, buf, path);

        glDeleteShader(*dst);
        *dst = 0;
        free(src);
        return -1;
    }

    free(src);
    return 0;
}

int shader_load(struct shader *dst, const char *vert_path, const char *frag_path){
    int status;
    dst->program = glCreateProgram();

    if(!dst->program){
        printf("[Error creating program]\n");
    }

    GLuint vert, frag;
    _shader_load(&vert, vert_path, GL_VERTEX_SHADER);
    _shader_load(&frag, frag_path, GL_FRAGMENT_SHADER);

    glAttachShader(dst->program, vert);
    glAttachShader(dst->program, frag);

    glLinkProgram(dst->program);

    glGetProgramiv(dst->program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE){
        int log_len = 0;
        glGetProgramiv(dst->program, GL_INFO_LOG_LENGTH, &log_len);

        char buf[log_len +1];
        glGetProgramInfoLog(dst->program, log_len, NULL, buf);

        printf("[Error linking program] %s\n", buf);

        glDetachShader(dst->program, vert);
        glDetachShader(dst->program, frag);

        glDeleteShader(vert);
        glDeleteShader(frag);

        glDeleteProgram(dst->program);
        dst->program = 0;

        return -1;
    }
    glDetachShader(dst->program, vert);
    glDetachShader(dst->program, frag);

    glDeleteShader(vert);
    glDeleteShader(frag);


    dst->attr_idx = 0;


    return 0;
}
void shader_free(struct shader *dst){
    glDeleteProgram(dst->program);
    dst->program = 0;
}
int shader_attach(struct shader *dst, const char *path, int shader_type){
    int status;
    dst->program = glCreateProgram();

    if(!dst->program){
        printf("[Error creating program]\n");
    }

    GLuint shader;
    _shader_load(&shader, path, shader_type);

    glAttachShader(dst->program, shader);

    glLinkProgram(dst->program);

    glGetProgramiv(dst->program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE){
        int log_len = 0;
        glGetProgramiv(dst->program, GL_INFO_LOG_LENGTH, &log_len);

        char buf[log_len +1];
        glGetProgramInfoLog(dst->program, log_len, NULL, buf);

        printf("[Error linking program] %s\n", buf);

        glDetachShader(dst->program, shader);

        glDeleteShader(shader);

        glDeleteProgram(dst->program);
        dst->program = 0;

        return -1;
    }
    glDetachShader(dst->program, shader);

    glDeleteShader(shader);
    return 0;
}

int shader_uniform_i(struct shader *dst, const char *name, const int src){
    GLCall(glUseProgram(dst->program));
    int location;
    GLCall(location = glGetUniformLocation(dst->program, name));
    GLCall(glUniform1i(location, src));
    return 0;
}
int shader_uniform_f(struct shader *dst, const char *name, const float src){
    GLCall(glUseProgram(dst->program));
    int location;
    GLCall(location = glGetUniformLocation(dst->program, name));
    GLCall(glUniform1f(location, src));
    return 0;
}
int shader_uniform_mat4f(struct shader *dst, const char *name, const float *src){
    GLCall(glUseProgram(dst->program));
    int location;
    GLCall(location = glGetUniformLocation(dst->program, name));
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, src));
    return 0;
}
int shader_attr_push(struct shader *dst, GLsizei num, GLenum type, GLboolean normalized, GLsizei stride, const void *offset){
    GLCall(glEnableVertexAttribArray(dst->attr_idx));
    GLCall(glVertexAttribPointer(dst->attr_idx, num, type, normalized, stride, offset));
    dst->attr_idx++;
    return 0;
}
int shader_attr_push_mat4f(struct shader *dst, GLboolean normalized, GLsizei stride, const void *offset){
    GLCall(glEnableVertexAttribArray(dst->attr_idx));
    GLCall(glVertexAttribPointer(dst->attr_idx, 4, GL_FLOAT, normalized, stride, offset + 0 * sizeof(float)));
    dst->attr_idx++;

    GLCall(glEnableVertexAttribArray(dst->attr_idx));
    GLCall(glVertexAttribPointer(dst->attr_idx, 4, GL_FLOAT, normalized, stride, offset + 4 * sizeof(float)));
    dst->attr_idx++;

    GLCall(glEnableVertexAttribArray(dst->attr_idx));
    GLCall(glVertexAttribPointer(dst->attr_idx, 4, GL_FLOAT, normalized, stride, offset + 8 * sizeof(float)));
    dst->attr_idx++;

    GLCall(glEnableVertexAttribArray(dst->attr_idx));
    GLCall(glVertexAttribPointer(dst->attr_idx, 4, GL_FLOAT, normalized, stride, offset + 12 * sizeof(float)));
    dst->attr_idx++;

    return 0;
}

int shader_attr_push_mat4f_div(struct shader *dst, GLboolean normalized, GLsizei stride, const void *offset){
    GLCall(glVertexAttribDivisor(dst->attr_idx, 1));
    GLCall(glEnableVertexAttribArray(dst->attr_idx));
    GLCall(glVertexAttribPointer(dst->attr_idx, 4, GL_FLOAT, normalized, stride, offset + 0 * sizeof(float)));
    dst->attr_idx++;

    GLCall(glVertexAttribDivisor(dst->attr_idx, 1));
    GLCall(glEnableVertexAttribArray(dst->attr_idx));
    GLCall(glVertexAttribPointer(dst->attr_idx, 4, GL_FLOAT, normalized, stride, offset + 4 * sizeof(float)));
    dst->attr_idx++;

    GLCall(glVertexAttribDivisor(dst->attr_idx, 1));
    GLCall(glEnableVertexAttribArray(dst->attr_idx));
    GLCall(glVertexAttribPointer(dst->attr_idx, 4, GL_FLOAT, normalized, stride, offset + 8 * sizeof(float)));
    dst->attr_idx++;

    GLCall(glVertexAttribDivisor(dst->attr_idx, 1));
    GLCall(glEnableVertexAttribArray(dst->attr_idx));
    GLCall(glVertexAttribPointer(dst->attr_idx, 4, GL_FLOAT, normalized, stride, offset + 12 * sizeof(float)));
    dst->attr_idx++;

    return 0;
}

int shader_bind(struct shader *src){
    GLCall(glUseProgram(src->program));
    return 0;
}
int shader_unbind(struct shader *src){
    GLCall(glUseProgram(0));
    return 0;
}

size_t attr_size(GLenum type){
    switch(type){
    case GL_BYTE:
        return 1;
    case GL_UNSIGNED_BYTE:
        return 1;
    case GL_SHORT:
        return 2;
    case GL_UNSIGNED_SHORT:
        return 2;
    case GL_INT:
        return 4;
    case GL_UNSIGNED_INT:
        return 4;
    case GL_FIXED:
        return 4;
    case GL_HALF_FLOAT:
        return 2;
    case GL_FLOAT:
        return 4;
    case GL_DOUBLE:
        return 8;
    }
    return 0;
}
