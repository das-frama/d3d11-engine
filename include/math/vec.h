#ifndef MOTOR_VEC_H
#define MOTOR_VEC_H

typedef struct {
    float x, y;
} vec2;

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float x, y, z, w;
} vec4;


vec2 vec2_new(void);
vec2 vec2_new(float, float);
vec2 vec2_add(vec2, vec2);
vec2 vec2_mul(vec2, float);

vec3 vec3_new(void);
vec3 vec3_new(float, float, float);
vec3 vec3_add(vec3, vec3);
vec3 vec3_mul(vec3, float);

vec4 vec4_new(void);
vec4 vec4_new(float, float, float, float);
vec4 vec4_add(vec4, vec4);
vec4 vec4_mul(vec4, float);
vec4 vec4_cross(vec4, vec4, vec4);

#endif // MOTOR_VEC_H