#include "math/vec.h"

#include <math.h>
#include <stdarg.h>
#include <assert.h>
#include <float.h>

/* Vec2 */

vec2 vec2_zero() {
	return vec2_new(0, 0);
}

vec2 vec2_new(float x, float y) {
	vec2 v;
	v.x = x;
	v.y = y;
	return v;
}

vec2 vec2_add(vec2 v1, vec2 v2) {
	vec2 v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	return v;
}

vec2 vec2_mul(vec2 v, float num) {
	v.x *= num;
	v.y *= num;
	return v;
}

/* Vec3 */

vec3 vec3_zero() {
	return vec3_new(0, 0, 0);
}

vec3 vec3_new(float x, float y, float z) {
	vec3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

vec3 vec3_add(vec3 v1, vec3 v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;
}

vec3 vec3_add_x(int n, ...) {
	assert(n >= 2);

	va_list args;
	va_start(args, n);

	vec3 v1 = va_arg(args, vec3);
	vec3 v2 = va_arg(args, vec3);
	vec3 v = vec3_add(v1, v2);
	for (int i = 0; i < n-2; i++) {
		vec3 temp = va_arg(args, vec3);
		v = vec3_add(v, temp);
	}

	va_end(args);
	return v;
}

vec3 vec3_mul(vec3 v, float num) {
	v.x *= num;
	v.y *= num;
	v.z *= num;
	return v;
}

vec3 vec3_div(vec3 v, float num) {
	v.x /= num;
	v.y /= num;
	v.z /= num;
	return v;
}

float vec3_length(vec3 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 vec3_normalize(vec3 v) {
	float len = vec3_length(v);
	if (len == 0.0f) {
		return vec3_zero();
	} else {
		return vec3_div(v, len);
	}
}

vec3 vec3_dir(float yaw, float pitch) {
	vec3 dir = {0};
	dir.x = cosf(yaw) * cosf(pitch);
	dir.y = sinf(pitch);
	dir.z = sinf(yaw) * cosf(pitch);

	return dir;
}

/* Vec4 */

vec4 vec4_zero() {
	return vec4_new(0, 0, 0, 0);
}

vec4 vec4_new(float x, float y, float z, float w) {
	return (vec4) { x, y, z, w };
}

vec4 vec4_new_vec3(vec3 v) {
	return (vec4) { v.x, v.y, v.z, 1.0f };
}

vec4 vec4_add(vec4 v1, vec4 v2) {
	vec4 v = { 0 };
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	v.w = v1.w + v2.w;
	return v;
}

vec4 vec4_mul(vec4 v, float num) {
	v.x *= num;
	v.y *= num;
	v.z *= num;
	v.w *= num;
	return v;
}

vec4 vec4_cross(vec4 v1, vec4 v2, vec4 v3) {
	vec4 v = { 0 };
    v.x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
	v.y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
	v.z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
	v.w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));

    return v;
}

quat quat_new(float x, float y, float z, float w) {
	quat q = { x, y, z, w };
	return q;
}

quat quat_id(void) {
	return quat_new(0, 0, 0, 1);
}

quat quat_from_euler(vec3 r) { // yaw (Z), pitch (Y), roll (X)
	float fc1 = cosf(r.z / 2.0f);
	float fc2 = cosf(r.x / 2.0f);
	float fc3 = cosf(r.y / 2.0f);

	float fs1 = sinf(r.z / 2.0f);
	float fs2 = sinf(r.x / 2.0f);
	float fs3 = sinf(r.y / 2.0f);

	return quat_new(
		fc1 * fc2 * fs3 - fs1 * fs2 * fc3,
		fc1 * fs2 * fc3 + fs1 * fc2 * fs3,
		fs1 * fc2 * fc3 - fc1 * fs2 * fs3,
		fc1 * fc2 * fc3 + fs1 * fs2 * fs3
	);
}

vec3 quat_to_euler(quat q) {
	float sqrx = q.x * q.x;
	float sqry = q.y * q.y;
	float sqrz = q.z * q.z;
	float sqrw = q.w * q.w;
  
  	return vec3_new(
	    asinf( -2.0f * ( q.x * q.z - q.y * q.w ) ),
	    atan2f( 2.0f * ( q.y * q.z + q.x * q.w ), (-sqrx - sqry + sqrz + sqrw) ),
	    atan2f( 2.0f * ( q.x * q.y + q.z * q.w ), ( sqrx - sqry - sqrz + sqrw) )
	);
}

float quat_length(quat q) {
  	return sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
}

quat quat_normalize(quat q) {
	float scale = quat_length(q);

	if (scale > FLT_EPSILON) {
		return quat_new(
		  q.x / scale,
		  q.y / scale,
		  q.z / scale,
		  q.w / scale);
	} else {
		return quat_new(0,0,0,0);
	}
}

quat quat_angle_axis(float angle, vec3 axis) {
	float sine = sinf(angle / 2.0f);
	float cosine = cosf(angle / 2.0f);

	return quat_normalize(quat_new(
		axis.x * sine,
		axis.y * sine,
		axis.z * sine,
		cosine)
	);
}

quat quat_rotation_x(float angle) {
  	return quat_angle_axis(angle, vec3_new(1,0,0));
}

quat quat_rotation_y(float angle) {
  	return quat_angle_axis(angle, vec3_new(0,1,0));
}

quat quat_rotation_z(float angle) {
  	return quat_angle_axis(angle, vec3_new(0,0,1));
}

quat quat_inverse(quat q) {
  float	scale = quat_length(q);
  quat result = quat_new(-q.x, -q.y, -q.z, q.w);

  if (scale > FLT_EPSILON) {    
    result.x /= scale;
    result.y /= scale;
    result.z /= scale;
    result.w /= scale;
  }
  
  return result;
}

quat quat_mul_quat(quat q1, quat q2) {
	return quat_new(
	    (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y),
	    (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x),
	    (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w),
	    (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z)
    );
}

vec3 quat_mul_vec3(quat q, vec3 v) {
	quat work = q;
	work = quat_mul_quat(work, quat_normalize(quat_new(v.x, v.y, v.z, 0.0)));
	work = quat_mul_quat(work, quat_inverse(q));

  	vec3 res = vec3_new(work.x, work.y, work.z);
  
  	return vec3_mul(res, vec3_length(v));
}

vec3 quat_forward(quat q) {
	vec3 v = {0};
	v.x = 2 * (q.x*q.z + q.w*q.y);
	v.y = 2 * (q.y*q.z - q.w*q.x);
	v.z = 1 - 2 * (q.x*q.x + q.y*q.y);

	return v;
}

vec3 quat_up(quat q) {
	vec3 v = {0};
	v.x = 2 * (q.x*q.y - q.w*q.z);
	v.y = 1 - 2 * (q.x*q.x + q.z*q.z);
	v.z = 2 * (q.y*q.z + q.w*q.x);

	return v;
}

vec3 quat_left(quat q) {
	vec3 v = {0};
	v.x = 1 - 2 * (q.y*q.y + q.z*q.z);
	v.y = 2 * (q.x*q.y + q.w*q.z);
	v.z = 2 * (q.x*q.z - q.w*q.y);

	return v;
}

