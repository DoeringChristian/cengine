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
