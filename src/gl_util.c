#include "gl_util.h"

void gl_err_clear(){
    while(glGetError() != GL_NO_ERROR);
}
void gl_err_print(const char *file, const char *func, int line){
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR){
        printf("[OpenGL Error] \t(%x) ", err);
        printf("in file: %s in function: %s at line: %i\n", file, func, line);
        raise(SIGINT);\
    }
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
