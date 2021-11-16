#ifndef MOTOR_MAT_H
#define MOTOR_MAT_H

#include "vec.h"

typedef struct {
    float mat[4][4];
} mat4;

mat4 mat4_new(float mat[4][4]);
mat4 mat4_zero(void);
mat4 mat4_id(void);

mat4 mat4_translate(vec3 v);
mat4 mat4_translate_mat4(mat4 m, vec3 v);
mat4 mat4_scale(vec3 v);
mat4 mat4_scale_by(float f);
mat4 mat4_rotate_x(float x);
mat4 mat4_rotate_y(float y);
mat4 mat4_rotate_z(float z);
mat4 mat4_inverse(mat4 m);
float mat4_det(mat4 m);

mat4 mat4_orthographic(float w, float h, float znear, float zfar);
mat4 mat4_perspective(float fov, float aspect, float znear, float zfar);

mat4 mat4_mul(mat4 m1, mat4 m2);
mat4 mat4_mul_x(int n, ...);

vec3 mat4_translation(mat4 m);
vec3 mat4_z_direction(mat4 m);
vec3 mat4_y_direction(mat4 m);
vec3 mat4_x_direction(mat4 m);

void mat4_print(const mat4* m);

#endif // MOTOR_MAT_H