#ifndef MOTOR_MAT_H
#define MOTOR_MAT_H

#include "vec.h"

typedef struct {
    float mat[4][4];
} mat4;

mat4 mat4_new(void);
mat4 mat4_id(void);

void mat4_translate(mat4* m, vec3 v);
void mat4_scale(mat4* m, vec3 v);
void mat4_rotate_x(mat4* m, float x);
void mat4_rotate_y(mat4* m, float y);
void mat4_rotate_z(mat4* m, float z);
void mat4_ortho_lh(mat4* m, float w, float h, float znear, float zfar);
void mat4_perspective_fov_lh(mat4* m, float fov, float aspect, float znear, float zfar);
void mat4_inverse(mat4* m);

mat4 mat4_mul(mat4 m1, mat4 m2);

float mat4_det(mat4 m);
vec3 mat4_translation(mat4 m);
vec3 mat4_z_direction(mat4 m);
vec3 mat4_y_direction(mat4 m);
vec3 mat4_x_direction(mat4 m);

#endif // MOTOR_MAT_H