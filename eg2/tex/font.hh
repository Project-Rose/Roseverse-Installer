#pragma once

#include <map>
#include <vector>
#include <gx2/texture.h>
#include <coreinit/memory.h>
#include "eg2/libs/typedefs.h"
#include "eg2/libs/schrift.h"
#include "eg2/tex/raw.hh"

enum FontAlignment {
    FONT_ALIGN_LEFT,
    FONT_ALIGN_CENTER,
    FONT_ALIGN_RIGHT
};

typedef enum OSFontType {
    FONT_CAFECN = OS_SHAREDDATATYPE_FONT_CHINESE,
    FONT_CAFEKR = OS_SHAREDDATATYPE_FONT_KOREAN,
    FONT_CAFESTD = OS_SHAREDDATATYPE_FONT_STANDARD,
    FONT_CAFETW = OS_SHAREDDATATYPE_FONT_TAIWANESE,
    FONT_CAFEMX = OS_SHAREDDATATYPE_FONT_MAX
} OSFontType;

namespace Tex {
    class LatteFont {
        public:
            LatteFont(OSFontType type, f32 size);
            ~LatteFont(void);

            void SwitchRenderType(FontAlignment align);

            void SetColor(const F32Color& rgba);
            void SetColor(const Color& rgba);
            void SetColor(u32 rgba);

            void RenderText(const char* text, f32 x, f32 y, f32 w, f32 h);
            void RenderTextScaled(RenderingAspectType aspect, f32 ref_w, f32 ref_h, const char* text, f32 base_x, f32 base_y);
            void RenderTextCentered(f32 offsetX, f32 offsetY, f32 w, f32 h, const char* text);
            void RenderTextCenteredScaled(f32 offsetX, f32 offsetY, RenderingAspectType aspect, f32 ref_width, f32 ref_height, const char* text);
        private:
            struct LineInfo {
                const char* start;
                const char* end;
                f32 width;
            };

            SFT sft;
            SFT_Font* font = nullptr;
            F32Color color = {0.0f, 0.0f, 0.0f, 1.0f};
            std::vector<u8> fontBuffer;
            FontAlignment currentAlign = FONT_ALIGN_LEFT;
            std::map<u32, GX2Texture*> glyphCache;

            f32 MeasureText(const char* text);
            GX2Texture* GetOrCreateGlyph(u32 codepoint, SFT_GMetrics& m);
    };
}