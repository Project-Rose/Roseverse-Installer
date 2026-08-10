#include <whb/gfx.h>
#include <gx2/registers.h>
#include "eg2/libs/typedefs.h"
#include "eg2/render/frames.hh"
#include "eg2/tex/raw.hh"
#include "eg2/render/scan.hh"

namespace Render {
    RenderWhereIsGoing RenderGoingThru = RENDERING_GOING_NONE;

    static void SetBlendControl(GX2BlendMode e1, GX2BlendMode e2, GX2BlendCombineMode e3) { GX2SetBlendControl(GX2_RENDER_TARGET_0, e1, e2, e3, true, e1, e2, e3); }

    static void GX2Frame(void) {
        GX2SetAlphaTest(true, GX2_COMPARE_FUNC_GREATER, 0.0f);
        GX2SetDepthOnlyControl(false, false, GX2_COMPARE_FUNC_NEVER);
        GX2SetColorControl(GX2_LOGIC_OP_COPY, 0xFF, false, true);
        SetBlendControl(GX2_BLEND_MODE_SRC_ALPHA, GX2_BLEND_MODE_INV_SRC_ALPHA, GX2_BLEND_COMBINE_MODE_ADD);
        GX2SetCullOnlyControl(GX2_FRONT_FACE_CCW, false, false);
    }

    void BeginFrame(void) {
        Tex::Clean();
        WHBGfxBeginRender();
    }

    void EndFrame(void) { WHBGfxFinishRender(); }

    void BeginRenderTV(void) { WHBGfxBeginRenderTV(); }
    void BeginRenderDRC(void) { WHBGfxBeginRenderDRC(); }
    void FinishRenderTV(void) { WHBGfxFinishRenderTV(); }
    void FinishRenderDRC(void) { WHBGfxFinishRenderDRC(); }

    void SetBGColor(F32Color& c) { WHBGfxClearColor(c.r, c.g, c.b, c.a); }

    void DRC(__Function__ func, F32Color _BGcolor) {
        RenderGoingThru = RENDERING_GOING_DRC;
        BeginRenderDRC();
            Update(true);
            SetBGColor(_BGcolor);
            GX2Frame();
            if (func) func();
        FinishRenderDRC();
        RenderGoingThru = RENDERING_GOING_NONE;
    }

    void DRC(__Function__ func, u32 _BGcolor) { DRC(func, Tex::U322F32(_BGcolor)); }
    void DRC(__Function__ func, Color _BGcolor) { DRC(func, Tex::U82F32(_BGcolor)); }
    void DRC(__Function__ func) { DRC(func, Tex::U322F32(0x000000FF)); }

    void TV(__Function__ func, F32Color _BGcolor) {
        RenderGoingThru = RENDERING_GOING_TV;
        BeginRenderTV();
            Update(false);
            SetBGColor(_BGcolor);
            GX2Frame();
            if (func) func();
        FinishRenderTV();
        RenderGoingThru = RENDERING_GOING_NONE;
    }

    void TV(__Function__ func, u32 _BGcolor) { TV(func, Tex::U322F32(_BGcolor)); }
    void TV(__Function__ func, Color _BGcolor) { TV(func, Tex::U82F32(_BGcolor)); }
    void TV(__Function__ func) { TV(func, Tex::U322F32(0x000000FF)); }
}