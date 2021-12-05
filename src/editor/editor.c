#include "editor/editor.h"

#include "platform/win32_platform.h"
#include "renderer/d3d11_renderer.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D11_IMPLEMENTATION
#include "vendor/nuklear.h"
#include "vendor/nuklear_d3d11.h"
#include "vendor/nuklear_style.c"

#include <d3d11_1.h>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_INDEX_BUFFER 128 * 1024

static struct nk_context* g_ctx = NULL;
static struct nk_colorf g_bg = {0};
static int g_client_width = 0;
static int g_client_height = 0;

void editor_init() {
    ID3D11Device* device = d3d11_device();

    win32_size(&g_client_width, &g_client_height);
    g_ctx = nk_d3d11_init(device, g_client_width, g_client_height, MAX_VERTEX_BUFFER, MAX_INDEX_BUFFER);
    struct nk_font_atlas *atlas;
    nk_d3d11_font_stash_begin(&atlas);
    nk_d3d11_font_stash_end();

    set_style(g_ctx, THEME_DARK);

    g_bg.r = 0.10f, g_bg.g = 0.18f, g_bg.b = 0.24f, g_bg.a = 1.0f;
}

void editor_close(void) {
    nk_d3d11_shutdown();
}

void editor_input_begin() {
    nk_input_begin(g_ctx);
}

void editor_input_end() {
    nk_input_end(g_ctx);
}

void editor_update() {
    if (nk_begin(g_ctx, "Object Hierarchy", nk_rect(0, 0, 250, g_client_height - 300),
        NK_WINDOW_BORDER|NK_WINDOW_TITLE))
    {

    }
    nk_end(g_ctx);

    if (nk_begin(g_ctx, "Asset Browser", nk_rect(0, g_client_height - 300, g_client_width - 350, 300), NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(g_ctx, 300, 2); // wrapping row

        if (nk_group_begin(g_ctx, "column1", NK_WINDOW_BORDER|NK_WINDOW_SCALE_LEFT)) { // column 1
            nk_layout_row_dynamic(g_ctx, 30, 1); // nested row
            // nk_label(g_ctx, "column 1.1", NK_TEXT_CENTERED);

            nk_layout_row_dynamic(g_ctx, 30, 1);
            // nk_label(g_ctx, "column 1.2", NK_TEXT_CENTERED);

            nk_group_end(g_ctx);
        }

        if (nk_group_begin(g_ctx, "column2", NK_WINDOW_BORDER|NK_WINDOW_SCALABLE)) { // column 2
            nk_layout_row_dynamic(g_ctx, 30, 1);
            // nk_label(g_ctx, "column 2.1", NK_TEXT_CENTERED);

            nk_layout_row_dynamic(g_ctx, 30, 1);
            // nk_label(g_ctx, "column 2.2", NK_TEXT_CENTERED);
            nk_group_end(g_ctx);
        }

    }
    nk_end(g_ctx);

    if (nk_begin(g_ctx, "Properties", nk_rect(g_client_width - 350, 0, 350, g_client_height),
        NK_WINDOW_BORDER|NK_WINDOW_TITLE))
    {

    }
    nk_end(g_ctx);

    if (nk_begin(g_ctx, "Run", nk_rect(250, 0, g_client_width - 600, 50),
        NK_WINDOW_BORDER))
    {
        // nk_layout_row_dynamic(g_ctx, 30, 0.5f);
        nk_button_label(g_ctx, "Play");
    }
    nk_end(g_ctx);
}

void editor_draw() {
    d3d11_reset_render_target();
    nk_d3d11_render(d3d11_context(), NK_ANTI_ALIASING_ON);
}