#pragma once

#include <gx2/texture.h>

enum HeapMEM1Sizes {
    MEM1_SIZE_START = 0xF4000000,
    MEM1_SIZE_END   = 0xF6000000
};

namespace GX2 {
    void AllocateTexture(GX2Texture* texture, bool allocateInMEM1);
    void DeallocateTexture(GX2Texture* texture);
}