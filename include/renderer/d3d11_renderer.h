#ifndef MOTOR_D3D11_RENDERER
#define MOTOR_D3D11_RENDERER

#include "engine.h"

typedef enum {
	RASTERIZER_CULL_FRONT,
	RASTERIZER_CULL_BACK,
	RASTERIZER_WIREFRAME,
} rasterizer_state;

typedef struct {
	size_t size_vertex;
    size_t size_list;
    ID3D11Buffer* buffer;
    ID3D11InputLayout* input_layout;
} d3d11_vertex_buffer;

typedef struct {
	size_t size_list;
	ID3D11Buffer* buffer;
} d3d11_index_buffer;

typedef ID3D11Buffer d3d11_buffer;

typedef ID3D11VertexShader d3d11_vertex_shader;
typedef ID3D11PixelShader d3d11_pixel_shader;

typedef struct {
	ID3D11Texture2D* tex;
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* smp;
} d3d11_texture;

void d3d11_init(int w, int h);
void d3d11_close(void);

void d3d11_create_device(void);
void d3d11_create_rasterizer_states(void);
void d3d11_create_swap_chain(int w, int h);
void d3d11_create_blend_state(void);

// Buffers.
void d3d11_reload_buffers(int w, int h);
void d3d11_set_rasterizer_state(rasterizer_state rstate);
void d3d11_clear_render_target_color(float r, float g, float b, float a);
void d3d11_set_viewport_size(int w, int h);
void d3d11_present(bool vsync);

// Vertex and Pixel shader.
d3d11_vertex_shader* d3d11_create_vertex_shader(const void* byte_code, size_t byte_code_size);
d3d11_pixel_shader* d3d11_create_pixel_shader(const void* byte_code, size_t byte_code_size);
void d3d11_release_vertex_shader(d3d11_vertex_shader* vs);
void d3d11_release_pixel_shader(d3d11_pixel_shader* ps);
void d3d11_compile_vertex_shader(const char* filename, const char* entry_point, void** byte_code, size_t* byte_code_size);
void d3d11_compile_pixel_shader(const char* filename, const char* entry_point, void** byte_code, size_t* byte_code_size);
void d3d11_release_compiled_shaders(void);
void d3d11_set_vertex_shader(d3d11_vertex_shader* s);
void d3d11_set_pixel_shader(d3d11_pixel_shader* s);

// Const buffer.
d3d11_buffer* d3d11_create_constant_buffer(void* data, size_t size);
void d3d11_update_constant_buffer(d3d11_buffer* cb, void* data);
void d3d11_release_constant_buffer(d3d11_buffer* cb);
void d3d11_vs_set_constant_buffer(d3d11_buffer* cb);
void d3d11_ps_set_constant_buffer(d3d11_buffer* cb);

// Vertex and index buffer.
d3d11_vertex_buffer* d3d11_create_vertex_buffer(void* vertices, size_t size_vertex, size_t size_list, void* byte_code, size_t size_byte_shader);
d3d11_index_buffer* d3d11_create_index_buffer(void* indices, size_t size_list);
void d3d11_release_vertex_buffer(d3d11_vertex_buffer* vb);
void d3d11_release_index_buffer(d3d11_index_buffer* ib);
void d3d11_set_vertex_buffer(d3d11_vertex_buffer* vb);
void d3d11_set_index_buffer(d3d11_index_buffer* ib);

// Texture.
d3d11_texture* d3d11_create_texture(byte* data, int w, int h, int n);
void d3d11_release_texture(d3d11_texture* t);
void d3d11_set_ps_texture(const d3d11_texture** textures, size_t texture_count);

// Draw
void d3d11_draw_indexed_triangle_list(size_t index_count, size_t start_index_location, size_t start_vertex_index);
void d3d11_draw_triangle_strip(size_t vertex_count, size_t start_vertex_index);
void d3d11_draw_indexed_line_list(size_t index_count, size_t start_index_location, size_t start_vertex_index);

#endif // MOTOR_D3D11_RENDERER