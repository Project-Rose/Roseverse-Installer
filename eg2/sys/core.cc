#include <coreinit/core.h>
#include <coreinit/thread.h>
#include "eg2/sys/report.hh"

namespace SYS {
    static const char* ThreadGet(void) {
        const char* name = OSGetThreadName(OSGetCurrentThread());
        return (name && name[0]) ? name : "Unk";
    }

    bool Init(void) {
        SYS::Report::Log("\n----------------------------------------------------------------------\n");
        SYS::Report::Log("EG2L BOOT TITLE\n");
        SYS::Report::Log("----------------------------------------------------------------------\n");
        SYS::Report::Log("Thread    : %s\n", ThreadGet());
        SYS::Report::Log("Core      : %u\n", OSGetCoreId());
        SYS::Report::Log("----------------------------------------------------------------------\n\n");

        return true;
    }

    bool Shutdown(void) {
        SYS::Report::Log("\n----------------------------------------------------------------------\n");
        SYS::Report::Log("EG2L SHUTDOWN TITLE\n");
        SYS::Report::Log("----------------------------------------------------------------------\n");

        return true;
    }
}