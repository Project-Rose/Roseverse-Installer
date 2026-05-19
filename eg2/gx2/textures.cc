#include <cstring>
#include <cstdlib>
#include <gx2/utils.h>
#include <gx2/texture.h>
#include "eg2/libs/typedefs.h"
#include "eg2/gx2/allocators.hh"
#include "eg2/gx2/textures.hh"
#include "eg2/heap/mem2.hh"

namespace GX2 {
    static void InitTexture(GX2Texture* texture, u32 width, u32 height, u32 depth, u32 nMips, GX2SurfaceFormat format, GX2SurfaceDim dim, GX2TileMode tileMode, u32 swizzle) {
        memset(texture, 0, sizeof(GX2Texture));

        texture->surface.dim       = dim;
        texture->surface.width     = width;
        texture->surface.height    = height;
        texture->surface.depth     = depth;
        texture->surface.mipLevels = nMips;
        texture->surface.format    = format;
        texture->surface.aa        = GX2_AA_MODE1X;
        texture->surface.use       = GX2_SURFACE_USE_TEXTURE;
        texture->surface.tileMode  = tileMode;
        texture->surface.swizzle   = swizzle;

        for (int i = 0; i < 13; ++i) texture->surface.mipLevelOffset[i] = 0;

        texture->viewFirstMip   = 0;
        texture->viewNumMips    = nMips;
        texture->viewFirstSlice = 0;
        texture->viewNumSlices  = depth;
        texture->compMap = 0x0010203; 

        for (int i = 0; i < 5; ++i) texture->regs[i] = 0;

        GX2CalcSurfaceSizeAndAlignment(&texture->surface);
        GX2InitTextureRegs(texture);
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