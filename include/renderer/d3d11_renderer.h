#ifndef MOTOR_D3D11_RENDERER
#define MOTOR_D3D11_RENDERER

/* Engine includes. */ 
#include "utils/utils.h"
#include "utils/log.h"

/* Window includes. */ 
#define D3D11_NO_HELPERS
#define COBJMACROS
#include <d3d11_1.h>

typedef struct {
	ID3D11Device* d3d_device;
	ID3D11DeviceContext* imm_context;
	// IDXGIDevice* dxgi_device;
	// IDXGIAdapter* dxgi_adapter;
	IDXGIFactory* dxgi_factory;

	ID3D11RasterizerState* cull_front_face;
	ID3D11RasterizerState* cull_back_face;

	IDXGISwapChain* swap_chain;
	ID3D11RenderTargetView* rtv;
	ID3D11DepthStencilView* dsv;
} renderer;

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

#endif // MOTOR_D3D11_RENDERER