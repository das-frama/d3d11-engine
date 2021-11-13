#include "renderer/d3d11_renderer.h"

#include "platform/win32_platform.h"

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#define SAFE_RELEASE(release, obj) if (obj) release##_Release(obj)
#define ID3DBlob_Release(b) if (b) b->lpVtbl->Release(b)
#define ID3DBlob_GetBufferPointer(b) b->lpVtbl->GetBufferPointer(b)
#define ID3DBlob_GetBufferSize(b) b->lpVtbl->GetBufferSize(b)

static ID3D11Device* g_d3d_device = NULL;
static ID3D11DeviceContext* g_imm_context = NULL;
static ID3D11DeviceContext1* g_imm_context1 = NULL;

static ID3D11RasterizerState* g_cull_front_face = NULL;
static ID3D11RasterizerState* g_cull_back_face = NULL;

static IDXGISwapChain* g_swap_chain = NULL;
static ID3D11RenderTargetView* g_rtv = NULL;
static ID3D11DepthStencilView* g_dsv = NULL;

static ID3DBlob* g_blob = NULL;

void d3d11_init(int w, int h) {
    // Create device.
    d3d11_create_device();

    // Create swap chain.
    d3d11_create_swap_chain(w, h);

    // Create rasterizer state.
    d3d11_create_rasterizer_state();

    // Reload buffers.
    d3d11_reload_buffers(w, h);
}

void d3d11_close() {
	if (g_imm_context) {
        ID3D11DeviceContext1_ClearState(g_imm_context);
    }

    SAFE_RELEASE(ID3D11Device, g_d3d_device);
    SAFE_RELEASE(ID3D11RasterizerState, g_cull_front_face);
    SAFE_RELEASE(ID3D11RasterizerState, g_cull_back_face);
    SAFE_RELEASE(ID3D11RenderTargetView, g_rtv);
    SAFE_RELEASE(ID3D11RenderTargetView, g_dsv);
    SAFE_RELEASE(IDXGISwapChain, g_swap_chain);
}

void d3d11_create_device() {
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    D3D_FEATURE_LEVEL level;

    HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 
        D3D11_CREATE_DEVICE_SINGLETHREADED, levels, _countof(levels), D3D11_SDK_VERSION, 
        &g_d3d_device, &level, &g_imm_context);
    if (FAILED(hr)) {
        error("D3D11CreateDevice error");
    }

    hr = ID3D11DeviceContext_QueryInterface(
        g_imm_context, &IID_ID3D11DeviceContext1,(void **) &g_imm_context1
    );
    if (FAILED(hr)) {
        error("ID3D11DeviceContext_QueryInterface error");
    }

    // using ID3D11DeviceContext1 to discard render target
    ID3D11DeviceContext_Release(g_imm_context);
    g_imm_context = (ID3D11DeviceContext*)g_imm_context1;
}

void d3d11_create_swap_chain(int w, int h) {
    IDXGIFactory* factory = NULL;
    HRESULT hr = CreateDXGIFactory(&IID_IDXGIFactory, (void **) &factory);
    if (FAILED(hr)) {
        error("CreateDXGIFactory error");
    }

    // Create swap chain.
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.BufferCount = 1;
    desc.BufferDesc.Width = w;
    desc.BufferDesc.Height = h;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = win32_window();
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    desc.Windowed = 1;

	hr = IDXGIFactory_CreateSwapChain(factory, (IUnknown*)g_d3d_device, &desc, &g_swap_chain);
    if (FAILED(hr)) {
        error("IDXGIFactory_CreateSwapChain error");
    }

    IDXGIFactory_Release(factory);
}

void d3d11_create_rasterizer_state() {
    D3D11_RASTERIZER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
    desc.CullMode = D3D11_CULL_FRONT;
    desc.DepthClipEnable = true;
    desc.FillMode = D3D11_FILL_SOLID;

    HRESULT hr = ID3D11Device_CreateRasterizerState(g_d3d_device, &desc, &g_cull_front_face);
    if (FAILED(hr)) {
        error("ID3D11Device_CreateRasterizerState error");
    }

    desc.CullMode = D3D11_CULL_BACK;
    hr = ID3D11Device_CreateRasterizerState(g_d3d_device, &desc, &g_cull_back_face);
    if (FAILED(hr)) {
        error("ID3D11Device_CreateRasterizerState error");
    }
}

void d3d11_reload_buffers(int w, int h) {
    ID3D11Texture2D* buffer = NULL;
    HRESULT hr = IDXGISwapChain_GetBuffer(g_swap_chain, 0, &IID_ID3D11Texture2D, (void **)&buffer);
    if (FAILED(hr)) {
        error("IDXGISwapChain_GetBuffer error");
    }

  	hr = ID3D11Device_CreateRenderTargetView(g_d3d_device, (ID3D11Resource*)buffer, NULL, &g_rtv);
   	ID3D11Texture2D_Release(buffer);
    if (FAILED(hr)) {
        error("ID3D11Device_CreateRenderTargetView error");
    }

    D3D11_TEXTURE2D_DESC tex_desc;
    ZeroMemory(&tex_desc, sizeof(D3D11_TEXTURE2D_DESC));
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

    hr = ID3D11Device_CreateTexture2D(g_d3d_device, &tex_desc, NULL, &buffer);
    if (FAILED(hr)) {
        error("ID3D11Device_CreateTexture2D error");
    }

	hr = ID3D11Device_CreateDepthStencilView(g_d3d_device, (ID3D11Resource*)buffer, NULL, &g_dsv);
    ID3D11Texture2D_Release(buffer);
    if (FAILED(hr)) {
        error("ID3D11Device_CreateDepthStencilView error");
    }
}

void d3d11_set_rasterizer_state(bool cull_front) {
     if (cull_front) {
        ID3D11DeviceContext_RSSetState(g_imm_context, g_cull_front_face);
    } else {
        ID3D11DeviceContext_RSSetState(g_imm_context, g_cull_back_face);
    }
}

void d3d11_clear_render_target_color(float r, float g, float b, float a) {
    float clear_color[] = {r, g, b, a};
  	ID3D11DeviceContext_ClearRenderTargetView(g_imm_context, g_rtv, clear_color);
  	ID3D11DeviceContext_ClearDepthStencilView(
        g_imm_context, g_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0
    );
    ID3D11DeviceContext_OMSetRenderTargets(g_imm_context, 1, &g_rtv, g_dsv);
}

void d3d11_set_viewport_size( int w, int h) {
    D3D11_VIEWPORT vp = {0};
    vp.Width = (float) w;
    vp.Height = (float) h;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

	ID3D11DeviceContext_RSSetViewports(g_imm_context, 1, &vp);
}

void d3d11_present(bool vsync) {
    IDXGISwapChain_Present(g_swap_chain, (UINT)vsync, 0);
}

void* d3d11_create_vertex_shader(const void* byte_code, size_t byte_code_size) {
    // shader* s = malloc(sizeof(shader));
    // memset(s, 0, sizeof(shader));
    ID3D11VertexShader* buffer = NULL;

    HRESULT hr = ID3D11Device_CreateVertexShader(g_d3d_device, byte_code, byte_code_size, NULL, &buffer);
    if (FAILED(hr)) {
        error("CreateVertexShader error");
    }

    return (void*)buffer;
}

void* d3d11_create_pixel_shader(const void* byte_code, size_t byte_code_size) {
    // shader* s = malloc(sizeof(shader));
    // memset(s, 0, sizeof(shader));
    ID3D11PixelShader* buffer = NULL;

    HRESULT hr = ID3D11Device_CreatePixelShader(g_d3d_device, byte_code, byte_code_size, NULL, &buffer);
    if (FAILED(hr)) {
        error("CreatePixelShader error");
    }

    return (void*)buffer;
}

void d3d11_release_vertex_shader(shader* s) {
    SAFE_RELEASE(ID3D11VertexShader, (ID3D11VertexShader*)s->ptr);

    free(s);
}

void d3d11_release_pixel_shader(shader* s) {
    SAFE_RELEASE(ID3D11PixelShader, (ID3D11PixelShader*)s->ptr);

    free(s);
}

void d3d11_compile_vertex_shader(
    const char* filename, const char* entry_point, void** byte_code, size_t* byte_code_size
) {
    ID3DBlob* error_blob = NULL;
    
    HRESULT hr = D3DCompileFromFile(
        to_wch(filename), NULL, NULL, entry_point, "vs_5_0", 0, 0, &g_blob, &error_blob
    );
    if (FAILED(hr)) {
        ID3DBlob_Release(error_blob);   
        error("D3DCompileFromFile error");
    }

    *byte_code = ID3DBlob_GetBufferPointer(g_blob);
    *byte_code_size = ID3DBlob_GetBufferSize(g_blob);
}

void d3d11_compile_pixel_shader(
    const char* filename, const char* entry_point, void** byte_code, size_t* byte_code_size
) {
    ID3DBlob* error_blob = NULL;

    HRESULT hr = D3DCompileFromFile(
        to_wch(filename), NULL, NULL, entry_point, "ps_5_0", 0, 0, &g_blob, &error_blob
    );
    if (FAILED(hr)) {
        ID3DBlob_Release(error_blob);
        error("D3DCompileFromFile error");
    }

    *byte_code = ID3DBlob_GetBufferPointer(g_blob);
    *byte_code_size = ID3DBlob_GetBufferSize(g_blob);
}

void d3d11_release_compiled_shaders() {
    ID3DBlob_Release(g_blob);
}

void d3d11_set_vertex_shader(shader* s) {
    ID3D11DeviceContext_VSSetShader(g_imm_context, (ID3D11VertexShader*)s->ptr, NULL, 0);
}

void d3d11_set_pixel_shader(shader* s) {
    ID3D11DeviceContext_PSSetShader(g_imm_context, (ID3D11PixelShader*)s->ptr, NULL, 0);
}

const_buffer* d3d11_create_const_buffer(void* data, size_t size) {
    const_buffer* cb = malloc(sizeof(const_buffer));
    memset(cb, 0, sizeof(const_buffer));

    D3D11_BUFFER_DESC buff_desc;
    ZeroMemory(&buff_desc, sizeof(D3D11_BUFFER_DESC));
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = size;
    buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    HRESULT hr = 0;
    if (data != NULL) {
        D3D11_SUBRESOURCE_DATA init_data;
        init_data.pSysMem = data;
        hr = ID3D11Device_CreateBuffer(g_d3d_device, &buff_desc, &init_data, &cb->buffer);
    } else {
        hr = ID3D11Device_CreateBuffer(g_d3d_device, &buff_desc, NULL, &cb->buffer);
    }
    if (FAILED(hr)) {
        error("ConstantBuffer not created successfully");
    }

    return cb;
}

void d3d11_update_const_buffer(const_buffer* cb, void* buffer) {
    ID3D11DeviceContext_UpdateSubresource(g_imm_context, (ID3D11Resource*)cb->buffer, 0, NULL, buffer, 0, 0);
}

void d3d11_release_const_buffer(const_buffer* cb) {
    SAFE_RELEASE(ID3D11Buffer, cb->buffer);

    free(cb);
}

void d3d11_vs_set_const_buffer(const_buffer* cb) {
    ID3D11DeviceContext_VSSetConstantBuffers(g_imm_context, 0, 1, &cb->buffer);
}

void d3d11_ps_set_const_buffer(const_buffer* cb) {
    ID3D11DeviceContext_PSSetConstantBuffers(g_imm_context, 0, 1, &cb->buffer);
}

vertex_buffer* d3d11_create_vertex_buffer(
    void* vertices, size_t size_vertex, size_t size_list, void* byte_code, size_t size_byte_shader
) {
    vertex_buffer* vb = malloc(sizeof(vertex_buffer));
    memset(vb, 0, sizeof(vertex_buffer));
    vb->size_vertex = size_vertex;
    vb->size_list = size_list;

    D3D11_BUFFER_DESC buff_desc;
    // ZeroMemory(&buff_desc, sizeof(D3D11_BUFFER_DESC));
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = size_vertex * size_list;
    buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data;
    // ZeroMemory(&init_data, sizeof(D3D11_SUBRESOURCE_DATA));
    init_data.pSysMem = vertices;

    HRESULT hr = ID3D11Device_CreateBuffer(g_d3d_device, &buff_desc, &init_data, &vb->buffer);
    if (FAILED(hr)) {
        error("CreateBuffer was not create successfully");
    }

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = ID3D11Device_CreateInputLayout(
        g_d3d_device, layout, _countof(layout), byte_code, size_byte_shader, &vb->input_layout
    );
    if (FAILED(hr)) {
        error("CreateInputLayout was not create successfully");
    }

    return vb;
}

index_buffer* d3d11_create_index_buffer(void* indices, size_t size_list) {
    index_buffer* ib = malloc(sizeof(index_buffer));
    memset(ib, 0, sizeof(index_buffer));
    ib->size_list = size_list;

    D3D11_BUFFER_DESC buff_desc;
    // ZeroMemory(&buff_desc, sizeof(D3D11_BUFFER_DESC));
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = sizeof(uint) * size_list;
    buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data;
    // ZeroMemory(&init_data, sizeof(D3D11_SUBRESOURCE_DATA));
    init_data.pSysMem = indices;

    HRESULT hr = ID3D11Device_CreateBuffer(g_d3d_device, &buff_desc, &init_data, &ib->buffer);
    if (FAILED(hr)) {
        error("IndexBuffer was not created successfully");
    }

    return ib;
}

void d3d11_release_vertex_buffer(vertex_buffer* vb) {
    SAFE_RELEASE(ID3D11Buffer, vb->buffer);
    SAFE_RELEASE(ID3D11InputLayout, vb->input_layout);

    free(vb);
}

void d3d11_release_index_buffer(index_buffer* ib) {
    SAFE_RELEASE(ID3D11Buffer, ib->buffer);

    free(ib);
}

void d3d11_set_vertex_buffer(vertex_buffer* vb) {
    UINT stride = vb->size_vertex;
    UINT offset = 0;
    ID3D11DeviceContext_IASetVertexBuffers(g_imm_context, 0, 1, &vb->buffer, &stride, &offset);
    ID3D11DeviceContext_IASetInputLayout(g_imm_context, vb->input_layout);
}

void d3d11_set_index_buffer(index_buffer* ib) {
    ID3D11DeviceContext_IASetIndexBuffer(g_imm_context, ib->buffer, DXGI_FORMAT_R32_UINT, 0);
}

texture* d3d11_create_texture(uchar* data, int w, int h, int n) {
    texture* t = malloc(sizeof(texture));
    memset(t, 0, sizeof(texture));

    // size_t row_pitch = (w * n + 7) / 8;
    size_t row_pitch = 4 * w * sizeof(uchar);  
    size_t image_size = row_pitch * h;

    // Create texture
    D3D11_TEXTURE2D_DESC tex_desc;
    ZeroMemory(&tex_desc, sizeof(D3D11_TEXTURE2D_DESC));
    tex_desc.Width = w;
    tex_desc.Height = h;
    tex_desc.MipLevels = 1;
    tex_desc.ArraySize = 1;
    tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tex_desc.SampleDesc.Count = 1;
    tex_desc.SampleDesc.Quality = 0;
    tex_desc.Usage = D3D11_USAGE_DEFAULT;
    tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    tex_desc.CPUAccessFlags = 0;
    tex_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data;
    init_data.pSysMem = data;
    init_data.SysMemPitch = row_pitch;
    init_data.SysMemSlicePitch = image_size;

    HRESULT hr = ID3D11Device_CreateTexture2D(g_d3d_device, &tex_desc, &init_data, &t->ptr);
    if (FAILED(hr)) {
        error("CreateTexture2D error");
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
    srv_desc.Format = tex_desc.Format;
    srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv_desc.Texture2D.MipLevels = 1;
    srv_desc.Texture2D.MostDetailedMip = 0;

    hr = ID3D11Device_CreateShaderResourceView(g_d3d_device, (ID3D11Resource*)t->ptr, &srv_desc, &t->srv);
    if (FAILED(hr)) {
       error("CreateShaderResourceView error");
    }

    D3D11_SAMPLER_DESC sampler_desc = { 0 };
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampler_desc.MinLOD = 0;
    sampler_desc.MaxLOD = 1;

    hr = ID3D11Device_CreateSamplerState(g_d3d_device, &sampler_desc, &t->smp);
    if (FAILED(hr)) {
       error("CreateSamplerState error");
    }

    ID3D11DeviceContext_GenerateMips(g_imm_context, t->srv);
    
    free(data);

    return t;
}

void d3d11_release_texture(texture* t) {
    SAFE_RELEASE(ID3D11Resource, (ID3D11Resource*)t->ptr);
    SAFE_RELEASE(ID3D11ShaderResourceView, (ID3D11ShaderResourceView*)t->srv);
    SAFE_RELEASE(ID3D11SamplerState, (ID3D11SamplerState*)t->smp);

    free(t);
}

void d3d11_set_ps_texture(const texture** t, size_t size) {
    ID3D11ShaderResourceView* list_res[32];
    ID3D11SamplerState* list_sampler[32];

    for (size_t i = 0; i < size; i++) {
        list_res[i] = t[i]->srv;
        list_sampler[i] = t[i]->smp;
    }

    ID3D11DeviceContext_PSSetShaderResources(g_imm_context, 0, size, list_res);
    ID3D11DeviceContext_PSSetSamplers(g_imm_context, 0, size, list_sampler);
}

void d3d11_draw_indexed_triangle_list(
    size_t index_count, size_t start_index_location, size_t start_vertex_index
) {
    ID3D11DeviceContext_IASetPrimitiveTopology(g_imm_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_DrawIndexed(g_imm_context, index_count, start_index_location, start_vertex_index);
}

void d3d11_draw_triangle_strip(size_t vertex_count, size_t start_vertex_index) {
    ID3D11DeviceContext_IASetPrimitiveTopology(g_imm_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    ID3D11DeviceContext_Draw(g_imm_context, vertex_count, start_vertex_index);
}