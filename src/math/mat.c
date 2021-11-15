#include "math/mat.h"
#include "math/vec.h"

/* Standard includes. */
#include <math.h>
#include <memory.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>

mat4 mat4_zero() {
	mat4 m = {0};
    memset(&m, 0, sizeof(float) * 16);
	return m;
}

mat4 mat4_new(float mat[4][4]) {
    mat4 m = {0};
    memcpy(m.mat, mat, sizeof(float) * 16);

    return m;
}

mat4 mat4_id() {
	mat4 m = mat4_zero();

	m.mat[0][0] = 1.0f;
	m.mat[1][1] = 1.0f;
	m.mat[2][2] = 1.0f;
	m.mat[3][3] = 1.0f;

	return m;
}

mat4 mat4_translate(vec3 v) {
    mat4 m = mat4_id();

    m.mat[3][0] = v.x;
    m.mat[3][1] = v.y;
    m.mat[3][2] = v.z;

    return m;
}

mat4 mat4_translate_mat4(mat4 m, vec3 v) {
    m.mat[3][0] = v.x;
    m.mat[3][1] = v.y;
    m.mat[3][2] = v.z;

    return m;
}

mat4 mat4_scale(vec3 v) {
    mat4 m = mat4_id();

    m.mat[0][0] = v.x;
    m.mat[1][1] = v.y;
    m.mat[2][2] = v.z;

    return m;
}

mat4 mat4_rotate_x(float x) {
    mat4 m = mat4_id();

    m.mat[1][1] = cos(x);
    m.mat[1][2] = sin(x);
    m.mat[2][1] = -sin(x);
    m.mat[2][2] = cos(x);

    return m;
}

mat4 mat4_rotate_y(float y) {
    mat4 m = mat4_id();

    m.mat[0][0] = cos(y);
    m.mat[0][2] = -sin(y);
    m.mat[2][0] = sin(y);
    m.mat[2][2] = cos(y);

    return m;
}

mat4 mat4_rotate_z(float z) {
    mat4 m = mat4_id();

    m.mat[0][0] = cos(z);
    m.mat[0][1] = sin(z);
    m.mat[1][0] = -sin(z);
    m.mat[1][1] = cos(z);

    return m;
}

mat4 mat4_orthographic(float w, float h, float znear, float zfar) {
    mat4 m = mat4_id();

    m.mat[0][0] = 2.0f / w;
    m.mat[1][1] = 2.0f / h;
    m.mat[2][2] = 1.0f / (zfar - znear);
    m.mat[3][2] = -(znear / (zfar - znear));

    return m;
}

mat4 mat4_perspective(float fov, float aspect, float znear, float zfar) {
    // float right = -(znear * tanf(fov));
    // float left = -right;

    // float top = aspect * znear * tanf(fov);
    // float bottom = -top;

    // mat4 m = mat4_zero();
    // m.mat[0][0] = (2.0 * znear) / (right - left);
    // m.mat[1][1] = (2.0 * znear) / (top - bottom);
    // m.mat[0][2] = (right + left) / (right - left);
    // m.mat[1][2] = (top + bottom) / (top - bottom);
    // m.mat[2][2] = (-zfar - znear) / (zfar - znear);
    // m.mat[3][2] = -1.0;
    // m.mat[2][3] = ( -(2.0 * znear) * zfar) / (zfar - znear);

    mat4 m = mat4_zero();

    float yscale = 1.0f / tan(fov / 2.0f);
    float xscale = yscale / aspect;

    m.mat[0][0] = xscale;
    m.mat[1][1] = yscale;
    m.mat[2][2] = zfar / (zfar - znear);
    m.mat[2][3] = 1.0f;
    m.mat[3][2] = (-znear * zfar) / (zfar - znear);

    return m;
}

mat4 mat4_inverse(mat4 m) {
    mat4 out = mat4_zero();
    vec4 v = vec4_zero(), vec[3] = {0};

    float det = mat4_det(m);
    if (det == 0.0f) return m;

    int a;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (j != i) {
                a = j;
                if (j > i) a = a - 1;
                vec[a].x = m.mat[j][0];
                vec[a].y = m.mat[j][1];
                vec[a].z = m.mat[j][2];
                vec[a].w = m.mat[j][3];
            }
        }
        v = vec4_cross(vec[0], vec[1], vec[2]);

        out.mat[0][i] = (float) pow(-1.0f, i) * v.x / det;
        out.mat[1][i] = (float) pow(-1.0f, i) * v.y / det;
        out.mat[2][i] = (float) pow(-1.0f, i) * v.z / det;
        out.mat[3][i] = (float) pow(-1.0f, i) * v.w / det;
    }

    return out;
}


mat4 mat4_mul(mat4 m1, mat4 m2) {
    mat4 out = mat4_new(m1.mat);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out.mat[i][j] =
                    m1.mat[i][0] * m2.mat[0][j] + m1.mat[i][1] * m2.mat[1][j] +
                    m1.mat[i][2] * m2.mat[2][j] + m1.mat[i][3] * m2.mat[3][j];
        }
    }

    return out;
}

mat4 mat4_mul_x(int n, ...) {
    assert(n >= 2);

    va_list args;
    va_start(args, n);

    mat4 m1 = va_arg(args, mat4);
    mat4 m2 = va_arg(args, mat4);
    mat4 m = mat4_mul(m1, m2);
    for (int i = 0; i < n-2; i++) {
        mat4 temp = va_arg(args, mat4);
        m = mat4_mul(m, temp);
    }

    va_end(args);
    return m;
}


float mat4_det(mat4 m) {
    vec4 minor = {0}, v1 = {0}, v2 = {0}, v3 = {0};

    v1 = vec4_new(m.mat[0][0], m.mat[1][0], m.mat[2][0], m.mat[3][0]);
    v2 = vec4_new(m.mat[0][1], m.mat[1][1], m.mat[2][1], m.mat[3][1]);
    v3 = vec4_new(m.mat[0][2], m.mat[1][2], m.mat[2][2], m.mat[3][2]);

    minor = vec4_cross(v1, v2, v3);

    return -(m.mat[0][3] * minor.x + m.mat[1][3] * minor.y + m.mat[2][3] * minor.z + m.mat[3][3] * minor.w);
}

vec3 mat4_translation(mat4 m) {
     return vec3_new(m.mat[3][0], m.mat[3][1], m.mat[3][2]);
 }

vec3 mat4_z_direction(mat4 m) {
    return vec3_new(m.mat[2][0], m.mat[2][1], m.mat[2][2]);
}

vec3 mat4_y_direction(mat4 m) {
    return vec3_new(m.mat[1][0], m.mat[1][1], m.mat[1][2]);
}

vec3 mat4_x_direction(mat4 m) {
    return vec3_new(m.mat[0][0], m.mat[0][1], m.mat[0][2]);
}

void mat4_print(const mat4* m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%.2f\t", m->mat[i][j]);
        }
        printf("\n");
    }
}