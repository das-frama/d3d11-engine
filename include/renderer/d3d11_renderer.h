#ifndef MOTOR_D3D11_RENDERER
#define MOTOR_D3D11_RENDERER

#include "engine.h"

typedef struct {
	ID3D11Device* d3d_device;
	ID3D11DeviceContext* imm_context;
	IDXGIFactory* dxgi_factory;

	ID3D11RasterizerState* cull_front_face;
	ID3D11RasterizerState* cull_back_face;

	IDXGISwapChain* swap_chain;
	ID3D11RenderTargetView* rtv;
	ID3D11DepthStencilView* dsv;
} renderer;

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
	ID3D11VertexShader* ptr;
} vertex_shader;

typedef struct {
	ID3D11PixelShader* ptr;
} pixel_shader;

renderer* d3d11_init(int w, int h);
void d3d11_close(renderer*);

void d3d11_create_device(renderer* r);
void d3d11_create_rasterizer_state(renderer*);
void d3d11_create_swap_chain(renderer*, int w, int h);

void d3d11_reload_buffers(renderer*, int w, int h);
void d3d11_set_rasterizer_state(renderer*, bool cull_front);
void d3d11_clear_render_target_color(renderer*, float r, float g, float b, float a);
void d3d11_set_viewport_size(renderer*, int w, int h);

void d3d11_present(renderer*, bool vsync);

vertex_shader d3d11_create_vertex_shader(const void* shader_byte_code, size_t byte_code_size);
pixel_shader d3d11_create_pixel_shader(const void* shader_byte_code, size_t byte_code_size);
void d3d11_compile_vertex_shader(const char* filename, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
void d3d11_compile_pixel_shader(const char* filename, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);

vertex_buffer d3d11_create_vertex_buffer(renderer* r, void* vertices, size_t size_vertex, size_t size_list, void* shader_byte_code, size_t size_byte_shader);
index_buffer d3d11_create_index_buffer(renderer* r, void* indices, size_t size_list);

#endif // MOTOR_D3D11_RENDERER