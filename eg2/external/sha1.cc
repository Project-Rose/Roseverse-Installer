#include <cstdio>
#include <string>
#include "eg2/libs/typedefs.h"
#include "eg2/libs/tinysha1.hh"
#include "eg2/external/sha1.hh"

namespace IO {
    namespace SHA1 {
        FileHashResult ReadFile(const char* path) {
            FILE* f = fopen(path, "rb");
            if (!f) return { 
                false, 
                "" 
            };

            sha1::SHA1 s;
            u8 buffer[4096];
            usize bytesRead;

            while ((bytesRead = fread(buffer, 1, sizeof(buffer), f)) > 0) s.processBytes(buffer, bytesRead);
            fclose(f);

            sha1::SHA1::digest8_t digest;
            s.getDigestBytes(digest);

            char out[41];
            for (int i = 0; i < 20; i++) snprintf(out + (i * 2), 3, "%02x", digest[i]);
            out[40] = '\0';

            return {
                true,
                std::string(out)
            };
        }
    }
}