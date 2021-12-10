#include "ui/ui_font.h"
#include "renderer/d3d11_renderer.h"
#include "shaders/font_vs.h"
#include "shaders/font_ps.h"
#include "vendor/stb_ds.h"
#include "vendor/stb_image.h"

#include <stdio.h>

ui_font* ui_font_load(const char* filename) {
    ui_font* font = calloc(1, sizeof(ui_font));

    filepath path = fp(filename);
    // Open and read the file.
    FILE* file = fopen(path.buf, "r");
    if (file == NULL) {
        error("cannot open file %s", path.buf);
    }

    // read info.
    fscanf(file, "info face=\"%s\" size=%hhu bold=%hhu italic=%hhu charset=\"*s\" unicode=%hhu stretchH=%hu smooth=%hhu aa=%hhu padding=%hhu,%hhu,%hhu,%hhu spacing=%hhu,%hhu outline=%hhu", 
           &font->face, &font->size, &font->bold, &font->italic, &font->unicode, &font->stretch_h, &font->smooth, &font->aa, &font->padding_top, &font->padding_right, &font->padding_bottom, &font->padding_left, &font->spacing_x, &font->spacing_y, &font->outline);

    // read common.
    fscanf(file, "common lineHeight=%hhu base=%hhu scaleW=%hu scaleH=%hu pages=%hhu packed=%hhu alphaChnl=%hhu redChnl=%hhu greenChnl=%hhu blueChnl=%hhu", &font->line_height, &font->base, &font->scale_w, &font->scale_h, &font->pages_count, &font->packed, &font->alpha_chnl, &font->red_chnl, &font->green_chnl, &font->blue_chnl);

    // read pages.
    font->pages = calloc(font->pages_count, sizeof(font_page));
    for (int i = 0; i < font->pages_count; i++) {
        fscanf(file, "page id=%hhu file=\"%s\"", &font->pages[i].page_id, &font->pages[i].file);
        fscanf(file, "chars count=%hu", &font->pages[i].chars_count);

        // load image.
        filepath font_map = fp(font->pages[i].file);
        font->pages[i].image_data = stbi_load(font_map.buf, NULL, NULL, NULL, 0);
        if (font->pages[i].image_data == NULL) {
            error("could not load image file");
        }

        font->pages[i].chars = NULL;
        for (int j = 0; j < font->pages[i].chars_count; j++) {
            font_char fc = {0};
            fscanf(file, "char id=%hu\tx=%hu\ty=%hu\twidth=%hu\theight=%hu\txoffset=%hd\tyoffset=%hd\txadvance=%hu\tpage=%hhu\tchnl=%hhu", &fc.key, &fc.x, &fc.y, &fc.width, &fc.height, &fc.xoffset, &fc.yoffset, &fc.xadvance, &fc.page, &fc.chnl);
            hmputs(font->pages[i].chars, fc);
        }
    }

    fclose(file);

    // Load shaders.
    font->vs = shader_new_byte((void*)g_font_vs, _countof(g_font_vs), SHADER_TYPE_VS);
    font->ps = shader_new_byte((void*)g_font_ps, _countof(g_font_ps), SHADER_TYPE_PS);

    return font;
}

void ui_font_delete(ui_font* font) {
    for (int i = 0; i < font->pages_count; i++) {
       hmfree(font->pages[i].chars);
    }
    free(font->pages);
    free(font);
}

void ui_font_draw(const ui_string* line, ui_font* font) {
    d3d11_set_rasterizer_state(RASTERIZER_CULL_BACK);
    // Set shaders.

    d3d11_set_vertex_shader((d3d11_vertex_shader*)font->vs->ptr);
    d3d11_set_pixel_shader((d3d11_pixel_shader*)font->ps->ptr);

    // Create index buffer.
    d3d11_set_vertex_buffer(line->vb);
    d3d11_set_index_buffer(line->ib);

    d3d11_draw_indexed_triangle_list(line->ib->size_list, 0, 0);
}

ui_string* ui_font_string_create(const char* line, const ui_font* font) {
    assert(font->pages_count > 0);
    assert(font->pages[0].chars_count > 0);

    ui_string* str = calloc(1, sizeof(ui_string));

    size_t len = strlen(line);

    vertex_font* vertices = calloc(len*6, sizeof(vertex_font));
    uint* indices = calloc(len*2, sizeof(uint));

    int index = 0;
    int draw_x = 0, draw_y = 0;
    for (int i = 0; i < len; i++) {
        font_char ch = hmgets(font->pages[0], (u16)line[i]);

        if (line[i] == ' ') {
            draw_x += ch.width + ch.xadvance; 
        }

        // create six triangles.
        // top left
        vertices[index].position = vec3_new(draw_x, draw_y, 0);
        vertices[index].texcoord = vec2_new(ch.x, ch.y);
        index += 1;

        vertices[index].position = vec3_new(draw_x, draw_y, 0);
        vertices[index].texcoord = vec2_new(ch.x, ch.y);
        index += 1;
    }
    // str->vb = ;

    return str;
}

void ui_font_string_delete(ui_string* str) {
    free(str);
}
