#pragma once

#include "eg2/libs/typedefs.h"
#include <string>

namespace Inkay {
    namespace Files {
        bool WriteWMS(const void* data, usize size);
        bool WriteWPS(const void* data, usize size);
    }
}