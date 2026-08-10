#pragma once

#include "eg2/tex/raw.hh"

enum RenderWhereIsGoing {
    RENDERING_GOING_NONE,
    RENDERING_GOING_TV,
    RENDERING_GOING_DRC
};

namespace Render {
    typedef void (*__Function__)(void);

    extern RenderWhereIsGoing RenderGoingThru;

    void BeginFrame(void);
    void EndFrame(void);

    void BeginRenderTV(void);
    void BeginRenderDRC(void);

    void FinishRenderTV(void);
    void FinishRenderDRC(void);

    void SetBGColor(F32Color& c);

    void DRC(__Function__ func, F32Color _BGcolor);
    void DRC(__Function__ func, u32 _BGcolor);
    void DRC(__Function__ func, Color _BGcolor);
    void DRC(__Function__ func);

    void TV(__Function__ func, F32Color _BGcolor);
    void TV(__Function__ func, u32 _BGcolor);
    void TV(__Function__ func, Color _BGcolor);
    void TV(__Function__ func);
}