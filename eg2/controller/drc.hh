#pragma once

#include <vpad/input.h>
#include "eg2/libs/typedefs.h"

namespace DRC {
    struct Vector2i {
        Vector2i(): x(0), y(0) {}
        Vector2i(s32 x, s32 y) : x(x), y(y) {}

        s32 x, y;
    };

    namespace Channel {
        inline constexpr u8 DRC_MAXCON = 2;
        inline constexpr VPADChan DRC0 = VPAD_CHAN_0;
        inline constexpr VPADChan DRC1 = VPAD_CHAN_1;
    }

    namespace Controller {
        extern VPADStatus DRCInput[Channel::DRC_MAXCON];
        extern VPADReadError DRCError[Channel::DRC_MAXCON];
    }

    namespace Input {
        inline constexpr u32 A          = VPAD_BUTTON_A;
        inline constexpr u32 B          = VPAD_BUTTON_B;
        inline constexpr u32 X          = VPAD_BUTTON_X;
        inline constexpr u32 Y          = VPAD_BUTTON_Y;
        inline constexpr u32 Left       = VPAD_BUTTON_LEFT;
        inline constexpr u32 Right      = VPAD_BUTTON_RIGHT;
        inline constexpr u32 Up         = VPAD_BUTTON_UP;
        inline constexpr u32 Down       = VPAD_BUTTON_DOWN;
        inline constexpr u32 ZL         = VPAD_BUTTON_ZL;
        inline constexpr u32 ZR         = VPAD_BUTTON_ZR;
        inline constexpr u32 L          = VPAD_BUTTON_L;
        inline constexpr u32 R          = VPAD_BUTTON_R;
        inline constexpr u32 Plus       = VPAD_BUTTON_PLUS;
        inline constexpr u32 Minus      = VPAD_BUTTON_MINUS;
        inline constexpr u32 Home       = VPAD_BUTTON_HOME;
        inline constexpr u32 Sync       = VPAD_BUTTON_SYNC;
        inline constexpr u32 TV         = VPAD_BUTTON_TV;
        inline constexpr u32 LS         = VPAD_BUTTON_STICK_L;
        inline constexpr u32 RS         = VPAD_BUTTON_STICK_R;
        inline constexpr u32 LS_Left    = VPAD_STICK_L_EMULATION_LEFT;
        inline constexpr u32 LS_Right   = VPAD_STICK_L_EMULATION_RIGHT;
        inline constexpr u32 LS_Up      = VPAD_STICK_L_EMULATION_UP;
        inline constexpr u32 LS_Down    = VPAD_STICK_L_EMULATION_DOWN;
        inline constexpr u32 RS_Left    = VPAD_STICK_R_EMULATION_LEFT;
        inline constexpr u32 RS_Right   = VPAD_STICK_R_EMULATION_RIGHT;
        inline constexpr u32 RS_Up      = VPAD_STICK_R_EMULATION_UP;
        inline constexpr u32 RS_Down    = VPAD_STICK_R_EMULATION_DOWN;
    }

    void Update(void);
    bool IsValidChannel(VPADChan ch);

    bool IsTouchInside(f32 x, f32 y, f32 w, f32 h);
    bool IsTouchInsideEx(f32 x, f32 y, f32 w, f32 h, VPADChan channel);

    bool IsTouchInsideSingle(f32 x, f32 y, f32 w, f32 h);
    bool IsTouchInsideSingleEx(f32 x, f32 y, f32 w, f32 h, VPADChan channel);

    bool ButtonHeld(u32 button);
    bool ButtonHeldEx(u32 button, VPADChan channel);

    bool ButtonTriggered(u32 button);
    bool ButtonTriggeredEx(u32 button, VPADChan channel);
}