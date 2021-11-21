#include "assets/texture.h"

#include "renderer/d3d11_renderer.h"
#include "vendor/stb_image.h"

texture* texture_new() {
    return (texture*) calloc(1, sizeof(texture));
}

texture* texture_new_load(const char* filename) {
    texture* tex = texture_new();
    
    byte* data = stbi_load(filename, &tex->w, &tex->h, &tex->bit, 4);
    if (data == NULL) {
    	error("could not load image file");
    }

    tex->ptr = (void*)d3d11_create_texture(data, tex->w, tex->h, tex->bit);
    return tex;
}

void texture_delete(texture* tex) {
    d3d11_release_texture((d3d11_texture*)tex->ptr);
    tex->ptr = NULL;
    free(tex);
}