#include <gx2/texture.h>
#include "eg2/libs/typedefs.h"
#include "eg2/gx2/allocators.hh"
#include "eg2/heap/mem1.hh"
#include "eg2/heap/mem2.hh"

namespace GX2 {
    void AllocateTexture(GX2Texture* texture, bool allocMEM1) {
        if (texture->surface.image != nullptr) return;

        if (allocMEM1) texture->surface.image = Heap::MEM1::Alloc(texture->surface.imageSize, texture->surface.alignment);
        else texture->surface.image = Heap::MEM2::Alloc(texture->surface.imageSize, texture->surface.alignment);
    }

    void DeallocateTexture(GX2Texture* texture) {
        if (!texture || !texture->surface.image) return;

        uptr addr = (uptr)texture->surface.image;

        if (addr >= MEM1_SIZE_START && addr < MEM1_SIZE_END) Heap::MEM1::Free(texture->surface.image);
        else Heap::MEM2::Free(texture->surface.image);

        texture->surface.image = nullptr;
    }
}