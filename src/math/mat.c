#include "math/mat.h"
#include "math/vec.h"

/* Standard includes. */
#include <math.h>
#include <memory.h>

mat4 mat4_new() {
	mat4 m = {0};
    memset(&m, 0, sizeof(float) * 16);
	return m;
}

mat4 mat4_id() {
	mat4 m = mat4_new();

	m.mat[0][0] = 1.0f;
	m.mat[1][1] = 1.0f;
	m.mat[2][2] = 1.0f;
	m.mat[3][3] = 1.0f;

	return m;
}

void mat4_translate(mat4* m, vec3 v) {
    m->mat[3][0] = v.x;
    m->mat[3][1] = v.y;
    m->mat[3][2] = v.z;
}

void mat4_scale(mat4* m, vec3 v) {
    m->mat[0][0] = v.x;
    m->mat[1][1] = v.y;
    m->mat[2][2] = v.z;
}

void mat4_rotate_x(mat4* m, float x) {
    m->mat[1][1] = cos(x);
    m->mat[1][2] = sin(x);
    m->mat[2][1] = -sin(x);
    m->mat[2][2] = cos(x);
}

void mat4_rotate_y(mat4* m, float y) {
    m->mat[0][0] = cos(y);
    m->mat[0][2] = -sin(y);
    m->mat[2][0] = sin(y);
    m->mat[2][2] = cos(y);
}

void mat4_rotation_z(mat4* m, float z) {
    m->mat[0][0] = cos(z);
    m->mat[0][1] = sin(z);
    m->mat[1][0] = -sin(z);
    m->mat[1][1] = cos(z);
}

void mat4_ortho_lh(mat4* m, float w, float h, float znear, float zfar) {
    m->mat[0][0] = 2.0f / w;
    m->mat[1][1] = 2.0f / h;
    m->mat[2][2] = 1.0f / (zfar - znear);
    m->mat[3][2] = -(znear / (zfar - znear));
}

void mat4_perspective_fov_lh(mat4* m, float fov, float aspect, float znear, float zfar) {
    float yscale = 1.0f / tan(fov / 2.0f);
    float xscale = yscale / aspect;

    m->mat[0][0] = xscale;
    m->mat[1][1] = yscale;
    m->mat[2][2] = zfar / (zfar - znear);
    m->mat[2][3] = 1.0f;
    m->mat[3][2] = (-znear * zfar) / (zfar - znear);
}

void mat4_inverse(mat4* m) {
    vec4 v, vec[3];
    int a, i, j;
    float det = 0.0f;

    det = mat4_det(*m);
    if (det == 0.0f) return m;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (j != i) {
                a = j;
                if (j > i) a = a - 1;
                vec[a].x = (m->mat[j][0]);
                vec[a].y = (m->mat[j][1]);
                vec[a].z = (m->mat[j][2]);
                vec[a].w = (m->mat[j][3]);
            }
        }
        vec4_cross(vec[0], vec[1], vec[2]);

        m->mat[0][i] = (float) pow(-1.0f, i) * v.x / det;
        m->mat[1][i] = (float) pow(-1.0f, i) * v.y / det;
        m->mat[2][i] = (float) pow(-1.0f, i) * v.z / det;
        m->mat[3][i] = (float) pow(-1.0f, i) * v.w / det;
    }
}


mat4 mat4_mul(mat4 m1, mat4 m2) {
    mat4 m = mat4_new();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.mat[i][j] = 
                m1.mat[i][0] * m2.mat[0][j] + 
                m1.mat[i][1] * m2.mat[1][j] + 
                m1.mat[i][2] * m2.mat[2][j] + 
                m1.mat[i][3] * m2.mat[3][j];
        }
    }

    return m;
}

float mat4_det(mat4 m) {
    float det = 0.0f;
    vec4 minor, v1, v2, v3;

    v1 = vec4_new(m.mat[0][0], m.mat[1][0], m.mat[2][0], m.mat[3][0]);
    v2 = vec4_new(m.mat[0][1], m.mat[1][1], m.mat[2][1], m.mat[3][1]);
    v3 = vec4_new(m.mat[0][2], m.mat[1][2], m.mat[2][2], m.mat[3][2]);

    minor = vec4_cross(v1, v2, v3);
    det = -(m.mat[0][3] * minor.x + m.mat[1][3] * minor.y + m.mat[2][3] * minor.z + m.mat[3][3] * minor.w);

    return det;
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