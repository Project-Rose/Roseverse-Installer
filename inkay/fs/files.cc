#include "eg2/libs/typedefs.h"
#include "eg2/external/core.hh"
#include "inkay/repos/download.hh"
#include "inkay/fs/files.hh"
#include <string>

namespace Inkay {
    namespace Files {
        static std::string GetWMS(void) { return "fs:/vol/external01/wiiu/environments/" + Inkay::Download::SelectedEnvironment + "/modules/Inkay-pretendo.wms"; }
        static std::string GetWPS(void) { return "fs:/vol/external01/wiiu/environments/" + Inkay::Download::SelectedEnvironment + "/plugins/Inkay-pretendo.wps"; }

        bool WriteWMS(const void* data, usize size) { 
            const std::string InkayWMS = GetWMS();
            return IO::WriteFile(InkayWMS.c_str(), data, size, FILEMODE_BIN);
        }

        bool WriteWPS(const void* data, usize size) { 
            const std::string InkayWPS = GetWPS();
            return IO::WriteFile(InkayWPS.c_str(), data, size, FILEMODE_BIN); 
        }
    }
}