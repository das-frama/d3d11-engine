#include "entities/light.h"

#include "vendor/stb_ds.h"

static light** lights = NULL;

light* light_new(void) {
    light* new_light = calloc(1, sizeof(light));
    arrput(lights, new_light);
    new_light->index = arrlen(lights) - 1;

    return new_light;
}

light* light_new_position(vec3 position) {
    light* new_light = light_new();
    new_light->position = position;

    return new_light;
}

void light_delete(light* l) {
    arrdel(lights, l->index);
    free(l);
}

void light_delete_all(void) {
    for (size_t i = 0; i < arrlen(lights); i++) {
        free(lights[i]);
    }

    arrfree(lights);
}