#include "camera.h"
#include "mathc.h"

int camera_init(struct camera *dst, int w, int h, float fov){
    //mat4_perspective_fov(dst->mat, fov, (float)w, (float)h, 0.1, 1000.0);
    //mat4_ortho(dst->mat, 0, 1, 0, 1, 0.1, 100);
    mat4_perspective(dst->mat, to_radians(45), (float)w/(float)h, 0.1, 100);
    //mat4_perspective_fov_t(dst->mat, fov, (float)w / (float)h, 0.1, 100);
#if 0
    for(size_t i = 0;i < 16;i++){
        printf("%f ", dst->mat[i]);
        if((i+1) % 4 == 0)
            printf("\n");
    }
#endif
    return 0;
}
void camera_free(struct camera *dst){

}
