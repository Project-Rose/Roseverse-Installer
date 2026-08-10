#include <cstdio>
#include <cstring>
#include <gx2/texture.h>
#include <gx2/mem.h>
#define STB_IMAGE_IMPLEMENTATION
#include "eg2/libs/stb_image.h"
#include "eg2/libs/typedefs.h"
#include "eg2/external/core.hh"
#include "eg2/gx2/textures.hh"
#include "eg2/heap/mem2.hh"

namespace IO {
    GX2Texture* JPGRead(const u8* data, usize length) {
        if (!data || length == 0) return nullptr;

        int width, height, channels;
        u8* pixels = stbi_load_from_memory(data, length, &width, &height, &channels, 4);
        if (!pixels) return nullptr;

        GX2Texture* texture = (GX2Texture*)(Heap::MEM2::Alloc(sizeof(GX2Texture), MEM_DEFAULT_ALIGN));
        if (!texture) {
            stbi_image_free(pixels);
            return nullptr;
        }

        GX2::CreateTexture(texture, width, height, 1, 1, GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,
            GX2_SURFACE_DIM_TEXTURE_2D, GX2_TILE_MODE_LINEAR_ALIGNED, 0, false);

        if (!texture->surface.image) {
            stbi_image_free(pixels);
            Heap::MEM2::Free(texture);
            return nullptr;
        }

        for (int y = 0; y < height; ++y) {
            u32* src = (u32*)(pixels + y * width * 4);
            u32* dst = (u32*)(texture->surface.image) + y * texture->surface.pitch;
            for (int x = 0; x < width; ++x) {
                u32 c = src[x];
                dst[x] = ((c & 0xFF0000) >> 16) | (c & 0xFF00FF00) | ((c & 0xFF) << 16);
            }
        }

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_TEXTURE, texture->surface.image, texture->surface.imageSize);
        stbi_image_free(pixels);
        return texture;
    }

    GX2Texture* PNGRead(const u8* data, usize length) {
        if (!data || length == 0) return nullptr;

        int width, height, channels;
        u8* pixels = stbi_load_from_memory(data, length, &width, &height, &channels, 4);
        if (!pixels) return nullptr;

        GX2Texture* texture = (GX2Texture*)(Heap::MEM2::Alloc(sizeof(GX2Texture), MEM_DEFAULT_ALIGN));
        if (!texture) {
            stbi_image_free(pixels);
            return nullptr;
        }

        GX2::CreateTexture(texture, width, height, 1, 1, GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8, 
            GX2_SURFACE_DIM_TEXTURE_2D, GX2_TILE_MODE_LINEAR_ALIGNED, 0, false);

        if (!texture->surface.image) {
            stbi_image_free(pixels);
            Heap::MEM2::Free(texture);
            return nullptr;
        }

        for (int y = 0; y < height; ++y) {
            u8* src = pixels + y * width * 4;
            u32* dst = (u32*)(texture->surface.image) + y * texture->surface.pitch;
            for (int x = 0; x < width; ++x) {
                u8 r = src[x * 4 + 0];
                u8 g = src[x * 4 + 1];
                u8 b = src[x * 4 + 2];
                u8 a = src[x * 4 + 3];
                dst[x] = (r << 24) | (g << 16) | (b << 8) | a;
            }
        }

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_TEXTURE, texture->surface.image, texture->surface.imageSize);

        stbi_image_free(pixels);
        return texture;
    }

    GX2Texture* GIFRead(const u8* data, usize length, int frameIndex) {
        if (!data || length == 0) return nullptr;

        int width, height, frameCount, channels;
        int* delays = nullptr;
        
        u8* pixels = stbi_load_gif_from_memory(data, length, &delays, &width, &height, &frameCount, &channels, 4);
        
        if (!pixels) return nullptr;

        if (frameIndex < 0 || frameIndex >= frameCount) frameIndex = 0;

        GX2Texture* texture = (GX2Texture*)(Heap::MEM2::Alloc(sizeof(GX2Texture), MEM_DEFAULT_ALIGN));
        if (!texture) {
            stbi_image_free(pixels);
            stbi_image_free(delays);
            return nullptr;
        }

        GX2::CreateTexture(texture, width, height, 1, 1, GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,
            GX2_SURFACE_DIM_TEXTURE_2D, GX2_TILE_MODE_LINEAR_ALIGNED, 0, false);

        if (!texture->surface.image) {
            stbi_image_free(pixels);
            stbi_image_free(delays);
            Heap::MEM2::Free(texture);
            return nullptr;
        }

        u8* frame_start = pixels + (frameIndex * width * height * 4);

        for (int y = 0; y < height; ++y) {
            u8* src_row = frame_start + (y * width * 4);
            u32* dst_row = (u32*)((u8*)texture->surface.image + (y * texture->surface.pitch));
            for (int x = 0; x < width; ++x) {
                u8 r = src_row[x * 4 + 0];
                u8 g = src_row[x * 4 + 1];
                u8 b = src_row[x * 4 + 2];
                u8 a = src_row[x * 4 + 3];
                dst_row[x] = (r << 24) | (g << 16) | (b << 8) | a;
            }
        }

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_TEXTURE, texture->surface.image, texture->surface.imageSize);

        stbi_image_free(pixels);
        stbi_image_free(delays); 
        
        return texture;
    }
}