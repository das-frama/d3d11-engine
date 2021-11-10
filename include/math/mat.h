#ifndef MOTOR_MAT_H
#define MOTOR_MAT_H

#include "vec.h"

typedef struct {
    float mat[4][4];
} mat4;

mat4 mat4_zero(void);
// mat4 mat4_new(float mat[4][4]);
mat4 mat4_id(void);

mat4 mat4_translation(mat4 m, vec3 v);
mat4 mat4_scale(mat4 m, vec3 v);
mat4 mat4_rotation_x(mat4 m, float x);
mat4 mat4_rotation_y(mat4 m, float y);
mat4 mat4_rotation_z(mat4 m, float z);

mat4 mat4_mul(mat4 m1, mat4 m2);
mat4 mat4_inverse(mat4 m);

float mat4_det(mat4 m);

mat4 mat4_ortho_lh(mat4 m, float width, float height, float near_plane, float far_plane);
mat4 mat4_perspective_fov_lh(mat4 m, float fov, float aspect, float znear, float zfar);

#endif // MOTOR_MAT_H