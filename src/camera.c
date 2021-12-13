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
int cverts_init_cube(struct cvert dst[6], vec3 pos, float far){
    cvert_init(&dst[0], 1, 1, glm_rad(90));
    glm_perspective(glm_rad(90), 1, 0.1, 100, dst[0].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, dst[1].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, dst[2].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, dst[3].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, dst[4].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, dst[5].proj);
    dst[0].far = far;
    dst[1].far = far;
    dst[2].far = far;
    dst[3].far = far;
    dst[4].far = far;
    dst[5].far = far;

    glm_look(pos, vec3(1,  0,  0),  vec3(0, -1, 0),  dst[0].view);
    glm_look(pos, vec3(-1, 0,  0),  vec3(0, -1, 0),  dst[1].view);
    glm_look(pos, vec3(0,  1,  0),  vec3(0, 0,  1),  dst[2].view);
    glm_look(pos, vec3(0,  -1, 0),  vec3(0, 0,  -1), dst[3].view);
    glm_look(pos, vec3(0,  0,  1),  vec3(0, -1, 0),  dst[4].view);
    glm_look(pos, vec3(0,  0,  -1), vec3(0, -1, 0),  dst[5].view);
    return 0;
}
