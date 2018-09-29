#pragma once

#include "Common.h"

//! Application state.
struct State {
    enum class NavItem {
        Mode,
        SubMode,
        Param0,
        Param1,
        Param2,
        Count,
    };

    enum class View {
        ScopeInput1,
        ScopeInput2,
        ScopeLayered,
        ScopeStacked,
        ScopeXY,
        LevelHistogram,
        LevelVU,
        LevelMeter,
        Invalid,
    };

    enum class Mode {
        Scope,
        Level,
        Freq,
        Count,
    };

    enum class Range {
        Bipolar6V,
        Bipolar12V,
        Unipolar6V,
        Unipolar12V,
        Count,
    };

    enum class TimeBase {
        T100us,
        T250us,
        T500us,
        T1ms,
        T2_5ms,
        T5ms,
        T10ms,
        T25ms,
        T50ms,
        T100ms,
        T250ms,
        T500ms,
        T1s,
        T2_5s,
        T5s,
        Count,
    };

    struct Scope {
        enum class Mode {
            Input1,
            Input2,
            Layered,
            Stacked,
            XY,
            Count,
        };

        Mode mode = Mode::Input1;
        TimeBase timeBase = TimeBase::T1ms;
        Range range = Range::Bipolar6V;
        // int16_t triggerMode;
    };

    struct Level {
        enum class Mode {
            Histogram,
            VU,
            Meter,
            Count,
        };

        struct Histogram {
            Range range = Range::Bipolar6V;
        };

        Mode mode = Mode::Histogram;
        Histogram histogram;
    };

    NavItem navItem = NavItem::Mode;
    Mode mode = Mode::Scope;
    Scope scope;
    Level level;

    View view() const {
        switch (mode) {
        case Mode::Scope:
            switch (scope.mode) {
            case Scope::Mode::Input1:       return View::ScopeInput1;
            case Scope::Mode::Input2:       return View::ScopeInput2;
            case Scope::Mode::Layered:      return View::ScopeLayered;
            case Scope::Mode::Stacked:      return View::ScopeStacked;
            case Scope::Mode::XY:           return View::ScopeXY;
            default: break;
            }
            break;
        case Mode::Level:
            switch (level.mode) {
            case Level::Mode::Histogram:    return View::LevelHistogram;
            case Level::Mode::VU:           return View::LevelVU;
            case Level::Mode::Meter:        return View::LevelMeter;
            default: break;
            }
            break;
        default: break;
        }
        return View::Invalid;
    }

    //! Increase value by one and return true if value has changed.
    template<typename Enum>
    static bool inc(Enum &value, int max = int(Enum::Count)) {
        Enum newValue = Enum(std::min(max - 1, int(value) + 1));
        bool changed = newValue != value;
        value = newValue;
        return changed;
    }

    //! Decrease value by one and return true if value has changed.
    template<typename Enum>
    static bool dec(Enum &value, int min = 0) {
        Enum newValue = Enum(std::max(min, int(value) - 1));
        bool changed = newValue != value;
        value = newValue;
        return changed;
    }

};
