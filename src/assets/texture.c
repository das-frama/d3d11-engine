#include "assets/texture.h"

#include "renderer/d3d11_renderer.h"
#include "vendor/stb_image.h"

texture* texture_load(const char* filename) {
	int w, h, bit;
    uchar *data = stbi_load(file_abs(filename), &w, &h, &bit, STBI_rgb_alpha);
    if (data == NULL) {
    	error("could not load image file");
    }

    texture* t = d3d11_create_texture(data, w, h, bit);

    stbi_image_free(data);
    return t;
}

void texture_unload(texture* t) {
	d3d11_release_texture(t);
    free(t);
}