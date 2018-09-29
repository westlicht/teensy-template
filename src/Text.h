#pragma once

#include "Common.h"
#include "State.h"

#define SIZE(_x_) (sizeof(_x_) / sizeof((_x_)[0]))

namespace Texts {

    static const char *Mode[]
    #if defined(_FORCE_PROGMEM__)
    PROGMEM
    #endif
    = {
        "SCOPE",
        "LEVELS",
        "FREQ",
    };
    static_assert(SIZE(Mode) == int(State::Mode::Count), "size mismatch");

    static const char *Range[]
    #if defined(_FORCE_PROGMEM__)
    PROGMEM
    #endif
    = {
        "+/-6V",
        "+/-12V",
        "+6V",
        "+12V",
    };
    static_assert(SIZE(Range) == int(State::Range::Count), "size mismatch");

    static const char *TimeBase[]
    #if defined(_FORCE_PROGMEM__)
    PROGMEM
    #endif
    = {
        "100us",
        "250us",
        "500us",
        "1ms",
        "2.5ms",
        "5ms",
        "10ms",
        "25ms",
        "50ms",
        "100ms",
        "250ms",
        "500ms",
        "1s",
        "2.5s",
        "5s",
    };
    static_assert(SIZE(TimeBase) == int(State::TimeBase::Count), "size mismatch");

    namespace Scope {

        static const char *Mode[]
        #if defined(_FORCE_PROGMEM__)
        PROGMEM
        #endif
        = {
            "Input 1",
            "Input 2",
            "Layered",
            "Stacked",
            "XY",
        };
        static_assert(SIZE(Mode) == int(State::Scope::Mode::Count), "size mismatch");

    } // namespace Scope

    namespace Level {

        static const char *Mode[]
        #if defined(_FORCE_PROGMEM__)
        PROGMEM
        #endif
        = {
            "Histogram",
            "VU",
            "Meter",
        };
        static_assert(SIZE(Mode) == int(State::Level::Mode::Count), "size mismatch");

    } // namespace Level

} // namespace Texts

