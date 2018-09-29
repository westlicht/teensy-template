#pragma once

#include "Common.h"
#include "Engine.h"
#include "State.h"
#include "Button.h"
#include "Theme.h"
#include "Painter.h"

#include <IntervalTimer.h>

class View;

//! Key codes.
enum class Key {
    Left,
    Right,
    Up,
    Down,
};

//! Main application class.
class App {
public:
    App();

    const Painter &painter() const { return _painter; }
          Painter &painter()       { return _painter; }

    const Engine &engine() const { return _engine; }
          Engine &engine()       { return _engine; }

    const State &state() const { return _state; }
          State &state()       { return _state; }

    const Theme &theme() const { return _theme; }

    void init();
    void run();

    void handleKey(Key key);

    void buttonTick();
    bool buttonNext(Key &key);

    static App &instance();

private:
    void changeView();

    Painter _painter;
    Engine _engine;
    IntervalTimer _buttonTimer;

    State _state;
    Button _buttons[4];
    Theme _theme;

    State::View _currentView;
    View *_view;

    uint32_t _targetInterval;
};
