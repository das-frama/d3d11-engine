#include "math/mat.h"
#include "math/vec.h"

/* Standard includes. */
#include <math.h>
#include <memory.h>

mat4 mat4_zero() {
	mat4 m = {0};
    memset(&m, 0, sizeof(float) * 16);
	return m;
}

// mat4 mat4_new(float mat[4][4]) {
// 	mat4 m = {0};
// 	m.mat = mat;
// 	return m;
// }

mat4 mat_id() {
	mat4 m = mat4_zero();

	m.mat[0][0] = 1.0f;
	m.mat[1][1] = 1.0f;
	m.mat[2][2] = 1.0f;
	m.mat[3][2] = 1.0f;

	return m;
}

mat4 mat4_translation(mat4 m, vec3 v) {
    m.mat[3][0] = v.x;
    m.mat[3][1] = v.y;
    m.mat[3][2] = v.z;

    return m;
}

mat4 mat4_scale(mat4 m, vec3 v) {
    m.mat[0][0] = v.x;
    m.mat[1][1] = v.y;
    m.mat[2][2] = v.z;

    return m;
}

mat4 mat4_rotation_x(mat4 m, float x) {
    m.mat[1][1] = cos(x);
    m.mat[1][2] = sin(x);
    m.mat[2][1] = -sin(x);
    m.mat[2][2] = cos(x);

    return m;
}

mat4 mat4_rotation_y(mat4 m, float y) {
    m.mat[0][0] = cos(y);
    m.mat[0][2] = -sin(y);
    m.mat[2][0] = sin(y);
    m.mat[2][2] = cos(y);

    return m;
}

mat4 mat4_rotation_z(mat4 m, float z) {
    m.mat[0][0] = cos(z);
    m.mat[0][1] = sin(z);
    m.mat[1][0] = -sin(z);
    m.mat[1][1] = cos(z);

    return m;
}

mat4 mat4_mul(mat4 m1, mat4 m2) {
    mat4 m = mat4_zero();

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

mat4 mat4_inverse(mat4 m) {
    vec4 v, vec[3];
    int a, i, j;
    float det = 0.0f;

    det = mat4_det(m);
    if (det == 0.0f) return m;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (j != i) {
                a = j;
                if (j > i) a = a - 1;
                vec[a].x = (m.mat[j][0]);
                vec[a].y = (m.mat[j][1]);
                vec[a].z = (m.mat[j][2]);
                vec[a].w = (m.mat[j][3]);
            }
        }
        vec4_cross(vec[0], vec[1], vec[2]);

        m.mat[0][i] = (float) pow(-1.0f, i) * v.x / det;
        m.mat[1][i] = (float) pow(-1.0f, i) * v.y / det;
        m.mat[2][i] = (float) pow(-1.0f, i) * v.z / det;
        m.mat[3][i] = (float) pow(-1.0f, i) * v.w / det;
    }

    return m;
}

mat4 mat4_ortho_lh(mat4 m, float width, float height, float near_plane, float far_plane) {
    m.mat[0][0] = 2.0f / width;
    m.mat[1][1] = 2.0f / height;
    m.mat[2][2] = 1.0f / (far_plane - near_plane);
    m.mat[3][2] = -(near_plane / (far_plane - near_plane));

    return m;
}

mat4 mat4_perspective_fov_lh(mat4 m, float fov, float aspect, float znear, float zfar) {
    float yscale = 1.0f / tan(fov / 2.0f);
    float xscale = yscale / aspect;

    m.mat[0][0] = xscale;
    m.mat[1][1] = yscale;
    m.mat[2][2] = zfar / (zfar - znear);
    m.mat[2][3] = 1.0f;
    m.mat[3][2] = (-znear * zfar) / (zfar - znear);

    return m;
}
