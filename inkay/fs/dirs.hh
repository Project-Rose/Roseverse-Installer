#pragma once

#include <vector>
#include <string>

namespace Inkay {
    namespace Dirs {
        extern std::vector<std::string> gEnvironments;
        extern int gSelectedEnv;

        void LoadEnvironments(void);
    }
}