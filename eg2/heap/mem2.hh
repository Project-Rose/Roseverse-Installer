#pragma once

#include "eg2/libs/typedefs.h"

enum GX2Heap {
    GX2_FRAME_HEAP = 0x123DECAF,
    GX2_POOL_SIZE = 0x400000
};

enum MEMAlign {
    MEM_DEFAULT_ALIGN = 64
};

namespace Heap {
    namespace MEM2 {
        void* Alloc(u32 size, u32 alignment);
        void Free(void* block);
    }
}