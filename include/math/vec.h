#ifndef MOTOR_VEC_H
#define MOTOR_VEC_H

#define color(r, g, b, a) vec4_new(r,g,b,a)

typedef struct {
    float x, y;
} vec2;

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float x, y, z, w;
} vec4;

vec2 vec2_zero(void);
vec2 vec2_new(float, float);
vec4 vec4_new_vec3(vec3);
vec2 vec2_add(vec2, vec2);
vec2 vec2_mul(vec2, float);

vec3 vec3_zero(void);
vec3 vec3_new(float, float, float);
vec3 vec3_add(vec3 v1, vec3 v2);
vec3 vec3_add_x(int n, ...);
vec3 vec3_mul(vec3, float);
vec3 vec3_div(vec3 v, float num);
float vec3_length(vec3 v);
vec3 vec3_normalize(vec3 v);

vec4 vec4_zero(void);
vec4 vec4_new(float, float, float, float);
vec4 vec4_add(vec4, vec4);
vec4 vec4_mul(vec4, float);
vec4 vec4_cross(vec4, vec4, vec4);

#endif // MOTOR_VEC_H