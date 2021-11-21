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

static ID3D11BlendState* g_blend_state = NULL;

static ID3D11RasterizerState* g_rasterer_frontface = NULL;
static ID3D11RasterizerState* g_rasterer_backface = NULL;
static ID3D11RasterizerState* g_rasterer_wireframe = NULL;

static IDXGISwapChain* g_swap_chain = NULL;
static ID3D11RenderTargetView* g_rtv = NULL;
static ID3D11DepthStencilView* g_dsv = NULL;

static ID3DBlob* g_blob = NULL;

void d3d11_init(int w, int h) {
    // Create device.
    d3d11_create_device();

    // Create swap chain.
    d3d11_create_swap_chain(w, h);

    // Create rasterizer states.
    d3d11_create_rasterizer_states();

    // Create blend state.
    d3d11_create_blend_state();

    // Reload buffers.
    d3d11_reload_buffers(w, h);
}

void d3d11_close() {
	if (g_imm_context) {
        ID3D11DeviceContext1_ClearState(g_imm_context);
    }

    SAFE_RELEASE(ID3D11Device, g_d3d_device);
    SAFE_RELEASE(ID3D11RasterizerState, g_rasterer_frontface);
    SAFE_RELEASE(ID3D11RasterizerState, g_rasterer_backface);
    SAFE_RELEASE(ID3D11RasterizerState, g_rasterer_wireframe);
    SAFE_RELEASE(ID3D11RenderTargetView, g_rtv);
    SAFE_RELEASE(ID3D11RenderTargetView, g_dsv);
    SAFE_RELEASE(IDXGISwapChain, g_swap_chain);
    SAFE_RELEASE(ID3D11BlendState, g_blend_state);
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

void d3d11_create_rasterizer_states() {
    D3D11_RASTERIZER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
    desc.CullMode = D3D11_CULL_FRONT;
    desc.DepthClipEnable = true;
    desc.FillMode = D3D11_FILL_SOLID;
    desc.AntialiasedLineEnable = true;
    desc.MultisampleEnable = true;

    // Front face.
    HRESULT hr = ID3D11Device_CreateRasterizerState(g_d3d_device, &desc, &g_rasterer_frontface);
    if (FAILED(hr)) {
        error("ID3D11Device_CreateRasterizerState error");
    }

    // Back face.
    desc.CullMode = D3D11_CULL_BACK;
    hr = ID3D11Device_CreateRasterizerState(g_d3d_device, &desc, &g_rasterer_backface);
    if (FAILED(hr)) {
        error("ID3D11Device_CreateRasterizerState error");
    }

    // Wireframe.
    desc.CullMode = D3D11_CULL_NONE;
    desc.FillMode = D3D11_FILL_WIREFRAME;
    hr = ID3D11Device_CreateRasterizerState(g_d3d_device, &desc, &g_rasterer_wireframe);
    if (FAILED(hr)) {
        error("ID3D11Device_CreateRasterizerState error");
    }
}

void d3d11_create_blend_state(void) {
    D3D11_BLEND_DESC desc;
    desc.AlphaToCoverageEnable = false;
    desc.IndependentBlendEnable = false;
    desc.RenderTarget[0].BlendEnable = true;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRESULT hr = ID3D11Device_CreateBlendState(g_d3d_device, &desc, &g_blend_state);
    if (FAILED(hr)) {
        error("ID3D11Device_CreateBlendState error");
    }

    float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    uint mask = 0xffffffff;
    ID3D11DeviceContext_OMSetBlendState(g_imm_context, g_blend_state, blend_factor, mask);
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

void d3d11_set_rasterizer_state(rasterizer_state rstate) {
    switch (rstate) {
        case RASTERIZER_CULL_FRONT:
            ID3D11DeviceContext_RSSetState(g_imm_context, g_rasterer_frontface);
            break;
        case RASTERIZER_CULL_BACK:
            ID3D11DeviceContext_RSSetState(g_imm_context, g_rasterer_backface);
            break;
        case RASTERIZER_WIREFRAME:
            ID3D11DeviceContext_RSSetState(g_imm_context, g_rasterer_wireframe);
            break;
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

d3d11_vertex_shader* d3d11_create_vertex_shader(const void* byte_code, size_t byte_code_size) {
    // shader* s = malloc(sizeof(shader));
    // memset(s, 0, sizeof(shader));
    d3d11_vertex_shader* shader = NULL;

    HRESULT hr = ID3D11Device_CreateVertexShader(g_d3d_device, byte_code, byte_code_size, NULL, &shader);
    if (FAILED(hr)) {
        error("CreateVertexShader error");
    }

    return shader;
}

d3d11_pixel_shader* d3d11_create_pixel_shader(const void* byte_code, size_t byte_code_size) {
    d3d11_pixel_shader* shader = NULL;

    HRESULT hr = ID3D11Device_CreatePixelShader(g_d3d_device, byte_code, byte_code_size, NULL, &shader);
    if (FAILED(hr)) {
        error("CreatePixelShader error");
    }

    return shader;
}

void d3d11_release_vertex_shader(d3d11_vertex_shader* vs) {
    SAFE_RELEASE(ID3D11VertexShader, vs);
}

void d3d11_release_pixel_shader(d3d11_pixel_shader* ps) {
    SAFE_RELEASE(ID3D11PixelShader, ps);
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

void d3d11_set_vertex_shader(d3d11_vertex_shader* vs) {
    ID3D11DeviceContext_VSSetShader(g_imm_context, vs, NULL, 0);
}

void d3d11_set_pixel_shader(d3d11_pixel_shader* ps) {
    ID3D11DeviceContext_PSSetShader(g_imm_context, ps, NULL, 0);
}

d3d11_buffer* d3d11_create_constant_buffer(void* data, size_t size) {
    d3d11_buffer* buffer = NULL;

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
        hr = ID3D11Device_CreateBuffer(g_d3d_device, &buff_desc, &init_data, &buffer);
    } else {
        hr = ID3D11Device_CreateBuffer(g_d3d_device, &buff_desc, NULL, &buffer);
    }
    if (FAILED(hr)) {
        error("ConstantBuffer not created successfully");
    }

    return buffer;
}

void d3d11_update_constant_buffer(d3d11_buffer* cb, void* data) {
    ID3D11DeviceContext_UpdateSubresource(g_imm_context, (ID3D11Resource*)cb, 0, NULL, data, 0, 0);
}

void d3d11_release_constant_buffer(d3d11_buffer* cb) {
    SAFE_RELEASE(ID3D11Buffer, cb);
}

void d3d11_vs_set_constant_buffer(d3d11_buffer* cb) {
    ID3D11DeviceContext_VSSetConstantBuffers(g_imm_context, 0, 1, &cb);
}

void d3d11_ps_set_constant_buffer(d3d11_buffer* cb) {
    ID3D11DeviceContext_PSSetConstantBuffers(g_imm_context, 0, 1, &cb);
}

d3d11_vertex_buffer* d3d11_create_vertex_buffer(
    void* vertices, size_t size_vertex, size_t size_list, void* byte_code, size_t size_byte_shader
) {
    d3d11_vertex_buffer* vb = malloc(sizeof(d3d11_vertex_buffer));
    memset(vb, 0, sizeof(d3d11_vertex_buffer));
    vb->size_vertex = size_vertex;
    vb->size_list = size_list;

    D3D11_BUFFER_DESC buff_desc;
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = size_vertex * size_list;
    buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data;
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

d3d11_index_buffer* d3d11_create_index_buffer(void* indices, size_t size_list) {
    d3d11_index_buffer* ib = malloc(sizeof(d3d11_index_buffer));
    memset(ib, 0, sizeof(d3d11_index_buffer));
    ib->size_list = size_list;

    D3D11_BUFFER_DESC buff_desc;
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = sizeof(uint) * size_list;
    buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data;
    init_data.pSysMem = indices;

    HRESULT hr = ID3D11Device_CreateBuffer(g_d3d_device, &buff_desc, &init_data, &ib->buffer);
    if (FAILED(hr)) {
        error("IndexBuffer was not created successfully");
    }

    return ib;
}

void d3d11_release_vertex_buffer(d3d11_vertex_buffer* vb) {
    SAFE_RELEASE(ID3D11Buffer, vb->buffer);
    SAFE_RELEASE(ID3D11InputLayout, vb->input_layout);

    free(vb);
}

void d3d11_release_index_buffer(d3d11_index_buffer* ib) {
    SAFE_RELEASE(ID3D11Buffer, ib->buffer);

    free(ib);
}

void d3d11_set_vertex_buffer(d3d11_vertex_buffer* vb) {
    UINT stride = vb->size_vertex;
    UINT offset = 0;
    ID3D11DeviceContext_IASetVertexBuffers(g_imm_context, 0, 1, &vb->buffer, &stride, &offset);
    ID3D11DeviceContext_IASetInputLayout(g_imm_context, vb->input_layout);
}

void d3d11_set_index_buffer(d3d11_index_buffer* ib) {
    ID3D11DeviceContext_IASetIndexBuffer(g_imm_context, ib->buffer, DXGI_FORMAT_R32_UINT, 0);
}

d3d11_texture* d3d11_create_texture(uchar* data, int w, int h, int n) {
    d3d11_texture* tex = malloc(sizeof(d3d11_texture));
    memset(tex, 0, sizeof(d3d11_texture));

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

    HRESULT hr = ID3D11Device_CreateTexture2D(g_d3d_device, &tex_desc, &init_data, &tex->tex);
    if (FAILED(hr)) {
        error("CreateTexture2D error");
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
    srv_desc.Format = tex_desc.Format;
    srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv_desc.Texture2D.MipLevels = 1;
    srv_desc.Texture2D.MostDetailedMip = 0;

    hr = ID3D11Device_CreateShaderResourceView(g_d3d_device, (ID3D11Resource*)tex->tex, &srv_desc, &tex->srv);
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

    hr = ID3D11Device_CreateSamplerState(g_d3d_device, &sampler_desc, &tex->smp);
    if (FAILED(hr)) {
       error("CreateSamplerState error");
    }

    ID3D11DeviceContext_GenerateMips(g_imm_context, tex->srv);
    
    free(data);

    return tex;
}

void d3d11_release_texture(d3d11_texture* tex) {
    SAFE_RELEASE(ID3D11Resource, (ID3D11Resource*)tex->tex);
    SAFE_RELEASE(ID3D11ShaderResourceView, (ID3D11ShaderResourceView*)tex->srv);
    SAFE_RELEASE(ID3D11SamplerState, (ID3D11SamplerState*)tex->smp);
}

void d3d11_set_ps_texture(const d3d11_texture** textures, size_t size) {
    ID3D11ShaderResourceView* list_res[32];
    ID3D11SamplerState* list_sampler[32];

    for (size_t i = 0; i < size; i++) {
        list_res[i] = textures[i]->srv;
        list_sampler[i] = textures[i]->smp;
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

void d3d11_draw_indexed_line_list(size_t index_count, size_t start_index_location, size_t start_vertex_index) {
    ID3D11DeviceContext_IASetPrimitiveTopology(g_imm_context, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    ID3D11DeviceContext_DrawIndexed(g_imm_context, index_count, start_index_location, start_vertex_index);
}