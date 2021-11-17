#ifndef MOTOR_D3D11_RENDERER
#define MOTOR_D3D11_RENDERER

#include "engine.h"

#include "assets/shader.h"
#include "assets/texture.h"

typedef struct {
	size_t size_vertex;
    size_t size_list;
    ID3D11Buffer* buffer;
    ID3D11InputLayout* input_layout;
} vertex_buffer;

typedef struct {
	size_t size_list;
	ID3D11Buffer* buffer;
} index_buffer;

typedef struct {
	ID3D11Buffer* buffer;
} const_buffer;

void d3d11_init(int w, int h);
void d3d11_close(void);

void d3d11_create_device(void);
void d3d11_create_rasterizer_state(void);
void d3d11_create_swap_chain(int w, int h);
void d3d11_create_blend_state(void);

// Buffers.
void d3d11_reload_buffers(int w, int h);
void d3d11_set_rasterizer_state(bool cull_front);
void d3d11_clear_render_target_color(float r, float g, float b, float a);
void d3d11_set_viewport_size(int w, int h);\
void d3d11_present(bool vsync);

// Vertex and Pixel shader.
void* d3d11_create_vertex_shader(const void* byte_code, size_t byte_code_size);
void* d3d11_create_pixel_shader(const void* byte_code, size_t byte_code_size);
void d3d11_release_vertex_shader(shader* s);
void d3d11_release_pixel_shader(shader* s);
void d3d11_compile_vertex_shader(const char* filename, const char* entry_point, void** byte_code, size_t* byte_code_size);
void d3d11_compile_pixel_shader(const char* filename, const char* entry_point, void** byte_code, size_t* byte_code_size);
void d3d11_release_compiled_shaders(void);
void d3d11_set_vertex_shader(shader* s);
void d3d11_set_pixel_shader(shader* s);

// Const buffer.
const_buffer* d3d11_create_const_buffer(void* data, size_t size);
void d3d11_update_const_buffer(const_buffer* cb, void* buffer);
void d3d11_release_const_buffer(const_buffer* cb);
void d3d11_vs_set_const_buffer(const_buffer* cb);
void d3d11_ps_set_const_buffer(const_buffer* cb);

// Vertex and index buffer.
vertex_buffer* d3d11_create_vertex_buffer(void* vertices, size_t size_vertex, size_t size_list, void* byte_code, size_t size_byte_shader);
vertex_buffer* d3d11_create_grid_vertex_buffer(void* vertices, size_t size_vertex, size_t size_list, void* byte_code, size_t size_byte_shader);
index_buffer* d3d11_create_index_buffer(void* indices, size_t size_list);
void d3d11_release_vertex_buffer(vertex_buffer* vb);
void d3d11_release_index_buffer(index_buffer* ib);
void d3d11_set_vertex_buffer(vertex_buffer* vb);
void d3d11_set_index_buffer(index_buffer* ib);

// Texture.
texture* d3d11_create_texture(uchar* data, int w, int h, int n);
void d3d11_release_texture(texture* t);
void d3d11_set_ps_texture(const texture** t, size_t size);

// Draw
void d3d11_draw_indexed_triangle_list(size_t index_count, size_t start_index_location, size_t start_vertex_index);
void d3d11_draw_triangle_strip(size_t vertex_count, size_t start_vertex_index);
void d3d11_draw_indexed_line_list(size_t index_count, size_t start_index_location, size_t start_vertex_index);
// void d3d11_draw_grid(int w, int h);

#endif // MOTOR_D3D11_RENDERER