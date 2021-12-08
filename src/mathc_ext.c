#include "mathc_ext.h"

mfloat_t *vec3_quat_rotate(mfloat_t *result, mfloat_t *v0, mfloat_t *q0){
    float q0_i[4];
    float result_q[4];
    float v0_q[4];
    vec4_assign(v0_q, v0);
    quat_inverse(q0_i, q0);

    quat_multiply(result_q, q0, v0_q);
    vec4_assign(v0_q, result_q);

    quat_multiply(result_q, v0_q, q0_i);
    return result;
}

mfloat_t *vec3_quat_rotate_inverse(mfloat_t *result, mfloat_t *v0, mfloat_t *q0){
    float q0_i[4];
    quat_inverse(q0_i, q0);
    return vec3_quat_rotate(result, v0, q0_i);
}
mfloat_t *mat4_perspective_fov_t(mfloat_t *result, mfloat_t fov, mfloat_t aspect, mfloat_t n, mfloat_t f){
    mfloat_t s = 1/ 1.0 / MTAN(fov/2);
    //mfloat_t tan_half_fov_y = MFLOAT_C(1.0) / MTAN(fov_y * MFLOAT_C(0.5));
    result[0] = s/aspect;
    result[1] = MFLOAT_C(0.0);
    result[2] = MFLOAT_C(0.0);
    result[3] = MFLOAT_C(0.0);
    result[4] = MFLOAT_C(0.0);
    result[5] = s;
    result[6] = MFLOAT_C(0.0);
    result[7] = MFLOAT_C(0.0);
    result[8] = MFLOAT_C(0.0);
    result[9] = MFLOAT_C(0.0);
    result[10] = -f / (f - n);
    result[11] = -MFLOAT_C(1.0);
    result[12] = MFLOAT_C(0.0);
    result[13] = MFLOAT_C(0.0);
    result[14] = -(f * n) / (f-n);
    result[15] = MFLOAT_C(0.0);
    return result;
}
