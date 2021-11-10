#include "math/vec.h"

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
	vec3 v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	return v;
}

vec3 vec3_mul(vec3 v, float num) {
	v.x *= num;
	v.y *= num;
	v.z *= num;
	return v;
}

/* Vec4 */

vec4 vec4_zero() {
	return vec4_new(0, 0, 0, 0);
}

vec4 vec4_new(float x, float y, float z, float w) {
	vec4 v = { 0 };
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
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
    v.x = v1.y * (v2.z * v3.w - v3.z * v2.w) -
    	  v1.z * (v2.y * v3.w - v3.y * v2.w) + 
    	  v1.w * (v2.y * v3.z - v2.z * v3.y);

    v.y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) -
            v1.z * (v2.x * v3.w - v3.x * v2.w) +
            v1.w * (v2.x * v3.z - v3.x * v2.z));

    v.z = v1.x * (v2.y * v3.w - v3.y * v2.w) - 
    	  v1.y * (v2.x * v3.w - v3.x * v2.w) +
          v1.w * (v2.x * v3.y - v3.x * v2.y);

    v.w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - 
    		v1.y * (v2.x * v3.z - v3.x * v2.z) +
            v1.z * (v2.x * v3.y - v3.x * v2.y));

    return v;
}
