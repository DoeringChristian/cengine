#include "gl_util.h"

void gl_err_clear(){
    while(glGetError() != GL_NO_ERROR);
}
void gl_err_print(const char *file, const char *func, int line){
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR){
        printf("[OpenGL Error] \t(%x) ", err);
        printf("in file: %s in function: %s at line: %i\n", file, func, line);
    }
}
