#include "entities/light.h"

light* light_new(void) {
    light* l = malloc(sizeof(light));
    memset(l, 0, sizeof(light));

    l->position = vec3_new(0, 0, 0);

    return l;
}

light* light_new_position(vec3 pos) {
    light* l = light_new();
    l->position = pos;

    return l;
}

void light_delete(light* l) {
    free(l);
}