#pragma once

#include <gx2/texture.h>
#include "eg2/libs/typedefs.h"

enum GX2MSAATypes {
    GX2_MSAA1X = GX2_AA_MODE1X,
    GX2_MSAA2X = GX2_AA_MODE2X,
    GX2_MSAA4X = GX2_AA_MODE4X,
    GX2_MSAA8X = GX2_AA_MODE8X,
};

enum GX2COMPMAPTypes {
    GX2_COMPMAP_RGBA = 0x0010203,
};

namespace GX2 {
    void CreateTexture(GX2Texture* tex, u32 w, u32 h, u32 depth, u32 nMips, GX2SurfaceFormat format, GX2SurfaceDim dim, GX2TileMode TM, u32 swizzle, bool m1);
    void UnloadTexture(GX2Texture* texture);
    void DestroyTexture(GX2Texture** tex);
}