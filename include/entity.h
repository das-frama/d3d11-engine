#ifndef MOTOR_ENTITY_H
#define MOTOR_ENTITY_H

#include "engine.h"

#include "assets/mesh.h"
#include "assets/material.h"

typedef uint entity_id;

/* Components. */
typedef struct {
    entity_id key;

    vec3 position;
    vec3 scale;
    quat rotation;
} transform;

typedef struct {
    entity_id key;

    vec3 velocity;
} movement;

typedef struct {
    entity_id key;

    mesh* mesh;
    material* mat;

    bool active; 
    bool recieve_shadows;
    bool cast_shadows;
} renderable;

typedef struct {
    entity_id key;

    mat4 world;
    mat4 view;
    mat4 proj;
    
    float fov;
    float znear, zfar;
    float w, h;
} camera;

typedef struct {
    entity_id key;

    float yaw, pitch, roll;
    float forward, rightward, topward;
    float speed;
} player;

typedef struct {
    transform* transforms;
    movement* movements;
    renderable* renderables;
    camera* cameras;
    player* players;
} component_list;

void entity_init(void);
void entity_close(void);

/* Entity. */
entity_id entity_create();
void entity_remove(entity_id id);

// int entity_register_system(void (*user_system)());
// void entity_unregister_system(int);

void entity_add_transform(entity_id id, vec3 pos, vec3 scale, quat rot);
void entity_add_movement(entity_id id, vec3 velocity);
void entity_add_renderable(entity_id id, mesh* mesh, material* mat);
void entity_add_camera(entity_id id, float fov, float znear, float zfar);
void entity_add_player(entity_id id);

transform* entity_get_transform(entity_id id);

void entity_delete_renderable(entity_id id);
void entity_delete_movement(entity_id id);
void entity_delete_transform(entity_id id);
void entity_delete_camera(entity_id id);

/* Systems. */
void entity_system_update();
void entity_player_control_system(player* p);
void entity_camera_system(camera* c);
void entity_render_system(renderable* r);

#endif // MOTOR_ENTITY_H