#include "renderer/d3d11_renderer.h"

#define SAFE_RELEASE(release, obj) if (obj) release##_Release(obj)

extern HWND window;

renderer* d3d11_init(int w, int h) {
	renderer* r = malloc(sizeof(renderer));

    // Create device.
    d3d11_create_device(r);

    // Create rasterizer state.
    d3d11_create_rasterizer_state(r);

    // Create swap chain.
    d3d11_create_swap_chain(r, w, h);

    // Reload buffers.
    d3d11_reload_buffers(r, w, h);

	// Set viewport size.
    d3d11_set_viewport_size(r, w, h);

    return r;
}

void d3d11_close(renderer* r) {
	if (r->imm_context) {
        ID3D11DeviceContext1_ClearState(r->imm_context);
    }

    // SAFE_RELEASE(IDXGIDevice, r->dxgi_device);
    SAFE_RELEASE(IDXGIFactory, r->dxgi_factory);
    SAFE_RELEASE(ID3D11Device, r->d3d_device);
    SAFE_RELEASE(ID3D11RasterizerState, r->cull_front_face);
    SAFE_RELEASE(ID3D11RasterizerState, r->cull_back_face);
    SAFE_RELEASE(ID3D11RenderTargetView, r->rtv);
    SAFE_RELEASE(ID3D11RenderTargetView, r->dsv);
    SAFE_RELEASE(IDXGISwapChain, r->swap_chain);
}

void d3d11_create_device(renderer* r) {
    HRESULT hr = D3D11CreateDevice(
        NULL, D3D_DRIVER_TYPE_HARDWARE, 
        NULL, 0, NULL, 0, 
        D3D11_SDK_VERSION, &r->d3d_device, NULL, &r->imm_context
    );
    if (FAILED(hr)) {
        error("d3d11 :: D3D11CreateDevice error");
    }

    hr = CreateDXGIFactory(&IID_IDXGIFactory,(void **) &r->dxgi_factory);
    if (FAILED(hr)) {
        error("d3d11 :: CreateDXGIFactory error");
    }
}

void d3d11_create_rasterizer_state(renderer* r) {
    D3D11_RASTERIZER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.CullMode = D3D11_CULL_FRONT;
    desc.DepthClipEnable = true;
    desc.FillMode = D3D11_FILL_SOLID;

    HRESULT hr = ID3D11Device_CreateRasterizerState(r->d3d_device, &desc, &r->cull_front_face);
    if (FAILED(hr)) {
    	error("d3d11 :: ID3D11Device_CreateRasterizerState error");
    }

    desc.CullMode = D3D11_CULL_BACK;
    hr = ID3D11Device_CreateRasterizerState(r->d3d_device, &desc, &r->cull_back_face);
    if (FAILED(hr)) {
    	error("d3d11 :: ID3D11Device_CreateRasterizerState error");
    }
}

void d3d11_create_swap_chain(renderer* r, int w, int h) {
    DXGI_SWAP_CHAIN_DESC desc;
    desc.BufferCount = 1;
    desc.BufferDesc.Width = w;
    desc.BufferDesc.Height = h;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = window;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    // desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.Windowed = 1;

	HRESULT hr = IDXGIFactory_CreateSwapChain(r->dxgi_factory, r->d3d_device, &desc, &r->swap_chain);
    if (FAILED(hr)) {
        error("d3d11 :: IDXGIFactory_CreateSwapChain error %d", hr);
    }
}

void d3d11_reload_buffers(renderer* r, int w, int h) {
    ID3D11Texture2D* buffer;
    HRESULT hr = IDXGISwapChain_GetBuffer(r->swap_chain, 0, &IID_ID3D11Texture2D, (void **)&buffer);
    if (FAILED(hr)) {
        error("d3d11 :: IDXGISwapChain_GetBuffer error");
    }

  	hr = ID3D11Device_CreateRenderTargetView(r->d3d_device, (ID3D11Resource*)buffer, NULL, &r->rtv);
   	ID3D11Texture2D_Release(buffer);
    if (FAILED(hr)) {
        error("d3d11 :: ID3D11Device_CreateRenderTargetView error");
    }

    D3D11_TEXTURE2D_DESC tex_desc;
    ZeroMemory(&tex_desc, sizeof(tex_desc));
    tex_desc.Width = w;
    tex_desc.Height = w;
    tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    tex_desc.Usage = D3D11_USAGE_DEFAULT;
    tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    tex_desc.MipLevels = 1;
    tex_desc.SampleDesc.Count = 1;
    tex_desc.SampleDesc.Quality = 0;
    tex_desc.MiscFlags = 0;
    tex_desc.ArraySize = 1;
    tex_desc.CPUAccessFlags = 0;

    hr = ID3D11Device_CreateTexture2D(r->d3d_device, &tex_desc, NULL, &buffer);
    if (FAILED(hr)) {
        error("d3d11 :: ID3D11Device_CreateTexture2D error");
    }

	hr = ID3D11Device_CreateDepthStencilView(r->d3d_device, (ID3D11Resource*)buffer, NULL, &r->dsv);
    ID3D11Texture2D_Release(buffer);
    if (FAILED(hr)) {
        error("d3d11 :: ID3D11Device_CreateDepthStencilView error");
    }
}

void d3d11_clear_render_target_color(renderer* renderer, float r, float g, float b, float a) {
    float clear_color[] = {r, g, b, a};
  	ID3D11DeviceContext_ClearRenderTargetView(renderer->imm_context, renderer->rtv, clear_color);
  	ID3D11DeviceContext_ClearDepthStencilView(renderer->imm_context, renderer->dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    ID3D11DeviceContext_OMSetRenderTargets(renderer->imm_context, 1, &renderer->rtv, renderer->dsv);
}

void d3d11_set_viewport_size(renderer* r, int w, int h) {
    D3D11_VIEWPORT vp = {0};
    vp.Width = (float) w;
    vp.Height = (float) h;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

	ID3D11DeviceContext_RSSetViewports(r->imm_context, 1, &vp);
}

void d3d11_present(renderer* r, bool vsync) {
	IDXGISwapChain_Present(r->swap_chain, (int)vsync, NULL);
    // r->swap_chain->Present(vsync, NULL);
}
