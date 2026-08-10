#include "eg2/external/core.hh"
#include "eg2/sys/report.hh"
#include <vector>
#include <string>

namespace Inkay {
    namespace Dirs {
        std::vector<std::string> gEnvironments;
        int gSelectedEnv = 0;

        void LoadEnvironments(void) {
            gEnvironments = IO::ListDirs("fs:/vol/external01/wiiu/environments");

            if (gEnvironments.empty()) {
                SYS::Report::Log("Inkay::Dirs::LoadEnvironments(): No environments found\n");
                gEnvironments.push_back("aroma");
            }

            gSelectedEnv = 0;

            if (gEnvironments.size() == 1) SYS::Report::Log("Inkay::Dirs::LoadEnvironments(): Only one environment found, auto-selecting: %s\n", gEnvironments[0].c_str());
        }
    }
}