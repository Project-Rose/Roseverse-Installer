#include <cstring>
#include <gx2/mem.h>
#include <gx2/enum.h>
#include <gx2/utils.h>
#include <coreinit/cache.h>
#include <coreinit/memory.h>
#include "eg2/libs/typedefs.h"
#define SCHRIFT_C
#include "eg2/libs/schrift.h"
#include "eg2/tex/font.hh"
#include "eg2/tex/raw.hh"
#include "eg2/render/scan.hh"
#include "eg2/gx2/textures.hh"
#include "eg2/sys/report.hh"
#include "eg2/external/core.hh"
#include "eg2/heap/mem2.hh"

namespace Tex {
    LatteFont::LatteFont(OSFontType type, f32 size) {
        void* sharedFont = nullptr;
        u32 sharedSize = 0;

        OSGetSharedData((OSSharedDataType)type, 0, &sharedFont, &sharedSize);

        if (sharedFont && sharedSize > 0) {
            font = sft_loadmem(sharedFont, sharedSize);
            OSMemoryBarrier();
        }

        sft.font = font;
        sft.xScale = size;
        sft.yScale = size;
        sft.flags = SFT_DOWNWARD_Y;
    }

    LatteFont::~LatteFont(void) {
        if (font) sft_freefont(font);

        for (auto const& [codepoint, tex] : glyphCache) GX2::UnloadTexture(tex);
        glyphCache.clear();
    }

    void LatteFont::SwitchRenderType(FontAlignment align) {
        if (!font) return;
        currentAlign = align; 
    }

    void LatteFont::SetColor(const F32Color& rgba) { this->color = rgba; }
    void LatteFont::SetColor(const Color& rgba) { this->color = Tex::U82F32(rgba); }
    void LatteFont::SetColor(u32 rgba) { this->color = Tex::U322F32(rgba); }

    f32 LatteFont::MeasureText(const char* text) {
        if (!font) return 0.0f;
        f32 maxW = 0, curW = 0;
        for (const char* p = text; *p; ++p) {
            if (*p == '\n') { if (curW > maxW) maxW = curW; curW = 0; continue; }
            SFT_GMetrics m; SFT_Glyph g;
            u32 cp = (*p == '\t') ? ' ' : (u32)*p;
            if (sft_lookup(&sft, cp, &g) == 0) {
                sft_gmetrics(&sft, g, &m);
                curW += (f32)m.advanceWidth * (*p == '\t' ? 4.0f : 1.0f);
            }
        }
        return (curW > maxW) ? curW : maxW;
    }

    GX2Texture* LatteFont::GetOrCreateGlyph(u32 codepoint, SFT_GMetrics& m) {
        if (!font) return nullptr;
        SFT_Glyph glyph;
        if (sft_lookup(&sft, codepoint, &glyph) < 0 || sft_gmetrics(&sft, glyph, &m) < 0) return nullptr;
        if (m.minWidth <= 0 || m.minHeight <= 0) return nullptr;

        if (glyphCache.count(codepoint)) return glyphCache[codepoint];

        std::vector<u8> rasterBuffer(m.minWidth * m.minHeight);
        SFT_Image img = {
            (void*)rasterBuffer.data(),
            m.minWidth,
            m.minHeight
        };
        if (sft_render(&sft, glyph, img) < 0) return nullptr;

        GX2Texture* tex = (GX2Texture*)Heap::MEM2::Alloc(sizeof(GX2Texture), MEM_DEFAULT_ALIGN);
        memset(tex, 0, sizeof(GX2Texture));

        GX2::CreateTexture(tex, m.minWidth, m.minHeight, 1, 1, GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8, 
            GX2_SURFACE_DIM_TEXTURE_2D, GX2_TILE_MODE_LINEAR_ALIGNED, 0, false);

        if (!tex->surface.image) {
            Heap::MEM2::Free(tex);
            return nullptr;
        }

        u8* dst = (u8*)tex->surface.image;
        u32 pitch = tex->surface.pitch;
        for (int y = 0; y < m.minHeight; ++y) {
            u32* row = (u32*)(dst + y * pitch * 4);
            for (int x = 0; x < m.minWidth; ++x) {
                u8 alpha = rasterBuffer[y * m.minWidth + x];
                row[x] = (0xFF << 24) | (0xFF << 16) | (0xFF << 8) | alpha;
            }
        }

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_TEXTURE, tex->surface.image, tex->surface.imageSize);
        OSMemoryBarrier();

        glyphCache[codepoint] = tex;
        return tex;
    }

    void LatteFont::RenderText(const char* text, f32 x, f32 y, f32 w, f32 h) {
        if (!font) return;
        if (!text || !*text) return;

        std::vector<LineInfo> lines;
        f32 naturalWidth = 0;
        const char* p = text;
        const char* lineStart = text;
        f32 currentLineWidth = 0;

        while (true) {
            if (*p == '\n' || *p == '\0') {
                lines.push_back({lineStart, p, currentLineWidth});
                if (currentLineWidth > naturalWidth) naturalWidth = currentLineWidth;
                currentLineWidth = 0;
                lineStart = p + 1;
                if (*p == '\0') break;
            } else {
                SFT_GMetrics m; SFT_Glyph g;
                u32 cp = (*p == '\t') ? ' ' : (u32)*p;
                if (sft_lookup(&sft, cp, &g) == 0) {
                    sft_gmetrics(&sft, g, &m);
                    currentLineWidth += (f32)m.advanceWidth * (*p == '\t' ? 4.0f : 1.0f);
                }
            }
            p++;
        }

        SFT_LMetrics lmtx;
        sft_lmetrics(&sft, &lmtx);
        f32 lineSpacing = (f32)(lmtx.ascender + lmtx.descender + lmtx.lineGap);
        f32 naturalHeight = lines.size() * lineSpacing;

        f32 scaleX = w / naturalWidth;
        f32 scaleY = h / naturalHeight;
        f32 fitScale = (scaleX < scaleY) ? scaleX : scaleY;

        f32 cursorY = y;

        for (const auto& line : lines) {
            f32 scaledLineWidth = line.width * fitScale;
            f32 alignmentOffsetX = 0.0f;

            if (currentAlign == FONT_ALIGN_CENTER) alignmentOffsetX = (w - scaledLineWidth) * 0.5f;
            else if (currentAlign == FONT_ALIGN_RIGHT) alignmentOffsetX = (w - scaledLineWidth);

            f32 cursorX = x + alignmentOffsetX;

            for (const char* curr = line.start; curr < line.end; ++curr) {
                if (*curr == '\t') {
                    SFT_GMetrics m; SFT_Glyph g;
                    sft_lookup(&sft, ' ', &g);
                    sft_gmetrics(&sft, g, &m);
                    cursorX += (f32)m.advanceWidth * 4.0f * fitScale;
                    continue;
                }

                SFT_GMetrics m;
                GX2Texture* tex = GetOrCreateGlyph((u32)*curr, m);
                if (tex) {
                    f32 drawX = cursorX + (f32)m.leftSideBearing * fitScale;
                    f32 drawY = cursorY + (f32)m.yOffset * fitScale;
                    RawPartialColor(tex, drawX, drawY, m.minWidth * fitScale, m.minHeight * fitScale, 0, 0, m.minWidth, m.minHeight, color.r, color.g, color.b, color.a);
                }
                cursorX += (f32)m.advanceWidth * fitScale;
            }
            cursorY += lineSpacing * fitScale;
        }
    }

    void LatteFont::RenderTextScaled(RenderingAspectType aspect, f32 ref_w, f32 ref_h, const char* text, f32 base_x, f32 base_y) {
        if (!font) return;
        const AspectRatio& ar = gAspectRatios[aspect];
        f32 sx = Render::gTargetWidth / ar.width;
        f32 sy = Render::gTargetHeight / ar.height;
        f32 sw = ref_w * sx;
        f32 sh = ref_h * sy;

        f32 natW = MeasureText(text);
        SFT_LMetrics lmtx;
        sft_lmetrics(&sft, &lmtx);
        f32 lineSpacing = (f32)(lmtx.ascender + lmtx.descender + lmtx.lineGap);
        
        int lineCount = 1;
        for (const char* c = text; *c; c++) if (*c == '\n') lineCount++;
        f32 natH = lineCount * lineSpacing;

        f32 fitScale = ((sw/natW) < (sh/natH)) ? (sw/natW) : (sh/natH);
        RenderText(text, base_x * sx, (base_y * sy) + (lmtx.ascender * fitScale), natW * fitScale, natH * fitScale);
    }

    void LatteFont::RenderTextCentered(f32 offsetX, f32 offsetY, f32 w, f32 h, const char* text) {
        if (!font) return;
        f32 natW = MeasureText(text);
        SFT_LMetrics lmtx;
        sft_lmetrics(&sft, &lmtx);
        f32 lineSpacing = (f32)(lmtx.ascender + lmtx.descender + lmtx.lineGap);

        int lineCount = 1;
        for (const char* c = text; *c; c++) if (*c == '\n') lineCount++;
        f32 natH = lineCount * lineSpacing;

        f32 fitScale = ((w/natW) < (h/natH)) ? (w/natW) : (h/natH);
        f32 drawW = natW * fitScale;
        f32 drawH = natH * fitScale;

        f32 x = ((Render::gTargetWidth - drawW) * 0.5f) + offsetX;
        f32 y = ((Render::gTargetHeight - drawH) * 0.5f) + (lmtx.ascender * fitScale) + offsetY;

        RenderText(text, x, y, drawW, drawH);
    }

    void LatteFont::RenderTextCenteredScaled(f32 offsetX, f32 offsetY, RenderingAspectType aspect, f32 ref_width, f32 ref_height, const char* text) {
        if (!font) return;
        const AspectRatio& ar = gAspectRatios[aspect];
        f32 sx = Render::gTargetWidth / ar.width;
        f32 sy = Render::gTargetHeight / ar.height;
        RenderTextCentered(offsetX * sx, offsetY * sy, ref_width * sx, ref_height * sy, text);
    }
}