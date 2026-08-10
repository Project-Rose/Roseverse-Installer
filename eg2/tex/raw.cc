#include <cstdlib>
#include <cstring>
#include <gx2/mem.h>
#include <gx2/sampler.h>
#include <gx2/draw.h>
#include <gx2/texture.h>
#include <gx2/shaders.h>
#include "eg2/libs/typedefs.h"
#include "eg2/tex/raw.hh"
#include "eg2/render/scan.hh"
#include "eg2/render/frames.hh"
#include "eg2/shaders/gsh.hh"
#include "eg2/heap/mem2.hh"

namespace Tex {
    static bool sampler_initialized = false;
    static void** tex_allocs = nullptr;
    static int num_allocs = 0;
    static int current_capacity = 0;
    static GX2Sampler SamplerWrap;
    static GX2Sampler SamplerClamp;

    static void SamplerInit(void) {
        if (!sampler_initialized) {
            GX2InitSampler(&SamplerClamp, GX2_TEX_CLAMP_MODE_CLAMP, GX2_TEX_XY_FILTER_MODE_LINEAR);
            GX2InitSampler(&SamplerWrap, GX2_TEX_CLAMP_MODE_WRAP, GX2_TEX_XY_FILTER_MODE_LINEAR);
            sampler_initialized = true;
        }
    }

    static void EnsureCapacity(void) {
        if ((num_allocs + 3) < current_capacity) return;

        int new_capacity = (current_capacity == 0) ? 4096 : current_capacity * 2;

        void** new_ptr = (void**)realloc(tex_allocs, new_capacity * sizeof(void*));
        if (new_ptr) {
            tex_allocs = new_ptr;
            current_capacity = new_capacity;
        }
    }

    static void AllocPush(void* ptr) {
        if (!ptr) return;
        EnsureCapacity();
        tex_allocs[num_allocs++] = ptr;
    }

    void Clean(void) {
        if (!tex_allocs) return;

        for (int i = 0; i < num_allocs; i++) {
            if (tex_allocs[i]) {
                free(tex_allocs[i]);
                tex_allocs[i] = nullptr;
            }
        }
        num_allocs = 0;
    }

    F32Color U322F32(u32 hex) { return { ((hex >> 24) & 0xFF) / 255.0f, ((hex >> 16) & 0xFF) / 255.0f, ((hex >> 8)  & 0xFF) / 255.0f, ((hex >> 0)  & 0xFF) / 255.0f }; }
    F32Color U82F32(const Color& c) { return { c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a }; }

    void RawQuadColor(f32 x, f32 y, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a) {
        if (!sampler_initialized) SamplerInit();

        f32 transform_x = (x / Render::gTargetWidth) * 2.0f - 1.0f;
        f32 transform_y = 1.0f - (y / Render::gTargetHeight) * 2.0f;
        f32 transform_width  = (w / Render::gTargetWidth) * 2.0f;
        f32 transform_height = (h / Render::gTargetHeight) * 2.0f;

        Shaders::Latte::Shader* gs = &Shaders::GSH::GSHColorGroup;
        GX2SetFetchShader(&gs->fetchShader);
        GX2SetVertexShader(gs->vertexShader);
        GX2SetPixelShader(gs->pixelShader);

        VTX vertices[4] = {
            {transform_x, transform_y, 0.0f},
            {transform_x + transform_width, transform_y, 0.0f},
            {transform_x + transform_width, transform_y - transform_height, 0.0f},
            {transform_x, transform_y - transform_height, 0.0f},
        };

        F32Color colors[4] = {
            {r, g, b, a},
            {r, g, b, a},
            {r, g, b, a},
            {r, g, b, a}
        };

        VTX* g_vertex_data      = (VTX*)(Heap::MEM2::Alloc(sizeof(vertices), MEM_DEFAULT_ALIGN));
        F32Color* g_color_data  = (F32Color*)(Heap::MEM2::Alloc(sizeof(colors), MEM_DEFAULT_ALIGN));

        memcpy(g_vertex_data, vertices, sizeof(vertices));
        memcpy(g_color_data, colors, sizeof(colors));

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_color_data, sizeof(colors));
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_vertex_data,sizeof(vertices));

        AllocPush(g_vertex_data);
        AllocPush(g_color_data);

        GX2SetAttribBuffer(0, sizeof(vertices), sizeof(VTX), g_vertex_data);
        GX2SetAttribBuffer(1, sizeof(colors),   sizeof(F32Color), g_color_data);

        GX2DrawEx(GX2_PRIMITIVE_MODE_QUADS, 4, 0, 1);
    }

    void RawPartialColor(GX2Texture* tex, f32 x, f32 y, f32 width, f32 height, int partialX, int partialY, int partialW, int partialH, f32 r, f32 g, f32 b, f32 a) {
        if (x > Render::gTargetWidth)  return;
        if (y > Render::gTargetHeight) return;
        if (x + width  < 0.0f) return;
        if (y + height < 0.0f) return;
        if (!sampler_initialized) SamplerInit();

        int totalW = tex->surface.width;
        int totalH = tex->surface.height;

        f32 tex_x_min = (f32)(partialX) / totalW;
        f32 tex_x_max = (f32)(partialX + partialW) / totalW;
        f32 tex_y_min = (f32)(partialY) / totalH;
        f32 tex_y_max = (f32)(partialY + partialH) / totalH;

        Shaders::Latte::Shader* gs = &Shaders::GSH::GSHTexturesGroup;
        GX2SetFetchShader(&gs->fetchShader);
        GX2SetVertexShader(gs->vertexShader);
        GX2SetPixelShader(gs->pixelShader);

        GX2SetPixelTexture(tex, 0);
        GX2SetPixelSampler(&SamplerClamp, 0);

        f32 transform_x = (x / Render::gTargetWidth) * 2.0f - 1.0f;
        f32 transform_y = 1.0f - (y / Render::gTargetHeight) * 2.0f;
        f32 transform_width  = (width / Render::gTargetWidth) * 2.0f;
        f32 transform_height = (height / Render::gTargetHeight) * 2.0f;

        VTX vertices[4] = {
            {transform_x, transform_y, 0.0f},
            {transform_x + transform_width, transform_y, 0.0f},
            {transform_x + transform_width, transform_y - transform_height, 0.0f},
            {transform_x, transform_y - transform_height, 0.0f},
        };

        UV uvs[4] = {
            {tex_x_min, tex_y_min},
            {tex_x_max, tex_y_min},
            {tex_x_max, tex_y_max},
            {tex_x_min, tex_y_max},
        };

        F32Color colors[4] = {
            {r, g, b, a},
            {r, g, b, a},
            {r, g, b, a},
            {r, g, b, a}
        };

        VTX* g_vertex_data      = (VTX*)(Heap::MEM2::Alloc(sizeof(vertices), MEM_DEFAULT_ALIGN));
        UV* g_tex_data          = (UV*)(Heap::MEM2::Alloc(sizeof(uvs), MEM_DEFAULT_ALIGN));
        F32Color* g_color_data  = (F32Color*)(Heap::MEM2::Alloc(sizeof(colors), MEM_DEFAULT_ALIGN));

        memcpy(g_vertex_data, vertices, sizeof(vertices));
        memcpy(g_tex_data, uvs, sizeof(uvs));
        memcpy(g_color_data, colors, sizeof(colors));

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_color_data, sizeof(colors));
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_tex_data,   sizeof(uvs));
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_vertex_data,sizeof(vertices));

        AllocPush(g_vertex_data);
        AllocPush(g_tex_data);
        AllocPush(g_color_data);

        GX2SetAttribBuffer(0, sizeof(vertices), sizeof(VTX), g_vertex_data);
        GX2SetAttribBuffer(1, sizeof(uvs),      sizeof(UV),     g_tex_data);
        GX2SetAttribBuffer(2, sizeof(colors),   sizeof(F32Color),  g_color_data);

        GX2DrawEx(GX2_PRIMITIVE_MODE_QUADS, 4, 0, 1);
    }

    void BackgroundRepeat(GX2Texture* tex) {
        f32 screenW = (f32)Render::gTargetWidth;
        f32 screenH = (f32)Render::gTargetHeight;

        f32 uvXMax = screenW / (f32)tex->surface.width;
        f32 uvYMax = screenH / (f32)tex->surface.height;

        Shaders::Latte::Shader* gs = &Shaders::GSH::GSHTexturesGroup;
        GX2SetFetchShader(&gs->fetchShader);
        GX2SetVertexShader(gs->vertexShader);
        GX2SetPixelShader(gs->pixelShader);

        GX2SetPixelTexture(tex, 0);
        GX2SetPixelSampler(&SamplerWrap, 0);

        VTX vertices[4] = {
            {-1,  1, 0},
            { 1,  1, 0},
            { 1, -1, 0},
            {-1, -1, 0},
        };

        UV uvs[4] = {
            {0.0f,   0.0f},
            {uvXMax, 0.0f},
            {uvXMax, uvYMax},
            {0.0f,   uvYMax},
        };

        F32Color colors[4] = {
            {1, 1, 1, 1},
            {1, 1, 1, 1},
            {1, 1, 1, 1},
            {1, 1, 1, 1}
        };

        VTX* g_vertex_data      = (VTX*)(Heap::MEM2::Alloc(sizeof(vertices), MEM_DEFAULT_ALIGN));
        UV* g_tex_data          = (UV*)(Heap::MEM2::Alloc(sizeof(uvs), MEM_DEFAULT_ALIGN));
        F32Color* g_color_data  = (F32Color*)(Heap::MEM2::Alloc(sizeof(colors), MEM_DEFAULT_ALIGN));

        memcpy(g_vertex_data, vertices, sizeof(vertices));
        memcpy(g_tex_data, uvs, sizeof(uvs));
        memcpy(g_color_data, colors, sizeof(colors));

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_color_data, sizeof(colors));
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_tex_data,   sizeof(uvs));
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_vertex_data,sizeof(vertices));

        AllocPush(g_vertex_data);
        AllocPush(g_tex_data);
        AllocPush(g_color_data);

        GX2SetAttribBuffer(0, sizeof(vertices), sizeof(VTX), g_vertex_data);
        GX2SetAttribBuffer(1, sizeof(uvs),      sizeof(UV),     g_tex_data);
        GX2SetAttribBuffer(2, sizeof(colors),   sizeof(F32Color),  g_color_data);

        GX2DrawEx(GX2_PRIMITIVE_MODE_QUADS, 4, 0, 1);
    }

    void BackgroundRepeatScaled(GX2Texture* tex, f32 sx, f32 sy) {
        f32 screenW = (f32)Render::gTargetWidth;
        f32 screenH = (f32)Render::gTargetHeight;

        f32 uvXMax = (screenW / (f32)tex->surface.width)  / sx;
        f32 uvYMax = (screenH / (f32)tex->surface.height) / sy;

        Shaders::Latte::Shader* gs = &Shaders::GSH::GSHTexturesGroup;
        GX2SetFetchShader(&gs->fetchShader);
        GX2SetVertexShader(gs->vertexShader);
        GX2SetPixelShader(gs->pixelShader);

        GX2SetPixelTexture(tex, 0);
        GX2SetPixelSampler(&SamplerWrap, 0);

        VTX vertices[4] = {
            {-1,  1, 0},
            { 1,  1, 0},
            { 1, -1, 0},
            {-1, -1, 0},
        };

        UV uvs[4] = {
            {0, 0},
            {uvXMax, 0},
            {uvXMax, uvYMax},
            {0, uvYMax},
        };

        F32Color colors[4] = {
            {1, 1, 1, 1},
            {1, 1, 1, 1},
            {1, 1, 1, 1},
            {1, 1, 1, 1}
        };

        VTX* g_vertex_data      = (VTX*)(Heap::MEM2::Alloc(sizeof(vertices), MEM_DEFAULT_ALIGN));
        UV* g_tex_data          = (UV*)(Heap::MEM2::Alloc(sizeof(uvs), MEM_DEFAULT_ALIGN));
        F32Color* g_color_data  = (F32Color*)(Heap::MEM2::Alloc(sizeof(colors), MEM_DEFAULT_ALIGN));

        memcpy(g_vertex_data, vertices, sizeof(vertices));
        memcpy(g_tex_data, uvs, sizeof(uvs));
        memcpy(g_color_data, colors, sizeof(colors));

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_color_data, sizeof(colors));
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_tex_data,   sizeof(uvs));
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER, g_vertex_data,sizeof(vertices));

        AllocPush(g_vertex_data);
        AllocPush(g_tex_data);
        AllocPush(g_color_data);

        GX2SetAttribBuffer(0, sizeof(vertices), sizeof(VTX), g_vertex_data);
        GX2SetAttribBuffer(1, sizeof(uvs),      sizeof(UV),     g_tex_data);
        GX2SetAttribBuffer(2, sizeof(colors),   sizeof(F32Color),  g_color_data);

        GX2DrawEx(GX2_PRIMITIVE_MODE_QUADS, 4, 0, 1);
    }
}