#include <whb/proc.h>
#include <whb/gfx.h>
#include <sysapp/launch.h>
#include <coreinit/launch.h>
#include "eg2/libs/typedefs.h"
#include "eg2/tex/raw.hh"
#include "eg2/sys/core.hh"
#include "eg2/shaders/gsh.hh"
#include "eg2/sys/report.hh"

namespace Render {
    void Init(void) {
        SYS::Init();
        WHBProcInit();
        WHBGfxInit();
        Shaders::Bind();
    }

    bool IsRunning(void) { return WHBProcIsRunning(); }
    void ExitToMenu(void) { SYSLaunchMenu(); }
    void StopRunning(void) { WHBProcStopRunning(); }

    void SoftRebootConsole(void) {
        OSForceFullRelaunch();
		ExitToMenu();
    }

    void FullyRebootConsole(void) { OSLaunchTitlel(OS_TITLE_ID_REBOOT, 0); }

    void Shutdown(void) {
        SYS::Shutdown();
        WHBGfxShutdown(); 
        WHBProcShutdown();
    }
}