#ifndef MOTOR_UI_FONT_H
#define MOTOR_UI_FONT_H

#include "engine.h"

#include "renderer/d3d11_renderer.h"
#include "assets/shader.h"

typedef struct {
    u16 key;
    u16 x, y;
    u16 width, height;
    s16 xoffset, yoffset;
    u16 xadvance;
    u8 page;
    u8 chnl;
} font_char;

typedef struct {
    u8 page_id;
    char file[32];
    
    u16 chars_count;
    font_char* chars;
    void* image_data;
} font_page;

typedef struct {
    // info
    char face[32];
    u8 size;
    u8 bold;
    u8 italic;
    u8 unicode;
    u16 stretch_h;
    u8 smooth;
    u8 aa;
    u8 padding_top, padding_bottom, padding_left, padding_right;
    u8 spacing_x, spacing_y;
    u8 outline;

    // common
    u8 line_height;
    u8 base;
    u16 scale_w, scale_h;
    u8 packed;
    u8 alpha_chnl, red_chnl, green_chnl, blue_chnl;

    u8 pages_count;
    font_page* pages;

    shader* vs;
    shader* ps;
} ui_font;

typedef struct {
    d3d11_vertex_buffer* vb;
    d3d11_index_buffer* ib;
    float r, g, b;
} ui_string;

typedef struct {
    vec3 position;
    vec2 texcoord;
} vertex_font;

ui_font* ui_font_load(const char* filename);
void ui_font_delete(ui_font* font);

void ui_font_draw(const ui_string* line, ui_font* font);

ui_string* ui_font_string_create(const char* line, const ui_font* font);
void ui_font_string_delete(ui_string* str);


#endif // MOTOR_UI_FONT_H