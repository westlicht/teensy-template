#pragma once

#include "Common.h"
#include "App.h"
#include "Point.h"
#include "Text.h"
#include "Painter.h"

static constexpr Point ScreenSize = Point(128, 160);
static constexpr Point ModePos = Point(2, 2);
static constexpr Point SubModePos = Point(66, 2);
static constexpr Point ParamPos[3] = { Point(2, 148), Point(45, 148), Point(87, 148) };

static constexpr int16_t TopNavHeight = 16;
static constexpr int16_t BottomNavHeight = 16;

class View {
public:
    View(App &app);
    virtual ~View();

    virtual int targetFPS() const { return 50; }

    virtual void drawNavigation();
    virtual void drawStatic();
    virtual void drawDynamic();

    struct ProcessContext {
        int begin;
        int end;
    };
    virtual void process(const ProcessContext &ctx);

    virtual bool handleKey(Key key);

protected:
    void setParamCount(uint8_t paramCount);
    
    virtual void drawHeader();
    virtual void drawFooter();

    void drawNavItem(const Point &pos, const char *text, bool highlighted);
    void clearContent();

    App &_app;
    Painter _painter;
    State &_state;
    const Theme &_theme;
    Engine &_engine;

    uint8_t _paramCount = 0;
};


template<typename Enum>
static bool handleLeftRight(Enum &value, Key key, int min = 0, int max = int(Enum::Count)) {
    if (key == Key::Left) {
        return State::dec(value, min);
    } else if (key == Key::Right) {
        return State::inc(value, max);
    }
    return false;
}

template<typename Enum>
static bool handleUpDown(Enum &value, Key key, int min = 0, int max = int(Enum::Count)) {
    if (key == Key::Up) {
        return State::dec(value, min);
    } else if(key == Key::Down) {
        return State::inc(value, max);
    }
    return false;
}
