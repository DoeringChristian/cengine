#ifndef MATHC_EXT_H
#define MATHC_EXT_H

#include "mathc.h"

mfloat_t *vec3_quat_rotate(mfloat_t *result, mfloat_t *v0, mfloat_t *q0);
mfloat_t *vec3_quat_rotate_inverse(mfloat_t *result, mfloat_t *v0, mfloat_t *q0);

#endif //MATHC_EXT_H
