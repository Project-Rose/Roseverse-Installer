#include <cstring>
#include <cstdlib>
#include <gx2/utils.h>
#include <gx2/texture.h>
#include "eg2/libs/typedefs.h"
#include "eg2/gx2/allocators.hh"
#include "eg2/gx2/textures.hh"
#include "eg2/heap/mem2.hh"
#include "eg2/render/frames.hh"

namespace GX2 {
    static void InitTexture(GX2Texture* tex, u32 width, u32 height, u32 depth, u32 nMips, GX2SurfaceFormat format, GX2SurfaceDim dim, GX2TileMode tileMode, u32 swizzle) {
        memset(tex, 0, sizeof(GX2Texture));

        tex->surface.dim       = dim;
        tex->surface.width     = width;
        tex->surface.height    = height;
        tex->surface.depth     = depth;
        tex->surface.mipLevels = nMips;
        tex->surface.format    = format;

        switch (Render::RenderGoingThru) {
            case RENDERING_GOING_TV:
                tex->surface.aa = GX2_AA_MODE2X;
                break;
            case RENDERING_GOING_DRC:
            case RENDERING_GOING_NONE:
            default:
                tex->surface.aa = GX2_AA_MODE1X;
                break;
        }

        tex->surface.use       = GX2_SURFACE_USE_TEXTURE;
        tex->surface.tileMode  = tileMode;
        tex->surface.swizzle   = swizzle;

        for (int i = 0; i < 13; ++i) tex->surface.mipLevelOffset[i] = 0;

        tex->viewFirstMip   = 0;
        tex->viewNumMips    = nMips;
        tex->viewFirstSlice = 0;
        tex->viewNumSlices  = depth;
        tex->compMap = GX2_COMPMAP_RGBA; 

        for (int i = 0; i < 5; ++i) tex->regs[i] = 0;

        GX2CalcSurfaceSizeAndAlignment(&tex->surface);
        GX2InitTextureRegs(tex);
    }

    void CreateTexture(GX2Texture* tex, u32 w, u32 h, u32 depth, u32 nMips, GX2SurfaceFormat format, GX2SurfaceDim dim, GX2TileMode TM, u32 swizzle, bool m1) {
        InitTexture(tex, w, h, depth, nMips, format, dim, TM, swizzle);
        GX2::AllocateTexture(tex, m1);
    }

    void UnloadTexture(GX2Texture* tex) {
        if (!tex) return;
        GX2::DeallocateTexture(tex);
        Heap::MEM2::Free(tex);
    }

    void DestroyTexture(GX2Texture** tex) {
        if (!tex || !*tex) return;

        UnloadTexture(*tex);
        *tex = nullptr;
    }
}