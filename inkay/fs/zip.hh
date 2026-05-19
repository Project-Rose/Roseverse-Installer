#pragma once

#include "eg2/libs/typedefs.h"
#include <vector>

namespace Inkay {
    namespace ZIP {
        bool Extract(const std::vector<u8>& zipData);
    }
}