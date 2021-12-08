#include "camera.h"

struct cvert cvert(int w, int h, float fov){
    struct cvert dst;
    dst.far = 100;
    glm_perspective(fov, (float)w/(float)h, 0.1, dst.far, dst.proj);
    glm_mat4_identity(dst.view);
    return dst;
}

int cvert_init(struct cvert *dst, int w, int h, float fov){
    dst->far = 100;
    glm_perspective(fov, (float)w / (float)h, 0.1, dst->far, dst->proj);
    glm_mat4_identity(dst->view);
#if 0
    for(size_t i = 0;i < 16;i++){
        printf("%f ", dst->mat[0][i]);
        if((i+1) % 4 == 0)
            printf("\n");
    }
#endif
    return 0;
}
void camera_free(struct cvert *dst){

}

void cvert_translate(struct cvert *dst, vec3 pos){
    glm_translate(dst->view, pos);
}
void cvert_rotate(struct cvert *dst, vec3 axis, float angle){
    glm_rotate(dst->view, angle, axis);
}
