#include "App.h"
#include "Config.h"

// Views
#include "View.h"
#include "ViewScope.h"
#include "ViewScopeSingle.h"
#include "ViewScopeLayered.h"
#include "ViewScopeStacked.h"
#include "ViewScopeXY.h"
#include "ViewLevel.h"
#include "ViewLevelHistogram.h"
#include "ViewLevelVU.h"
#include "ViewLevelMeter.h"

static void buttonISR() {
    App::instance().buttonTick();
}

//! Constructor.
App::App() :
    _painter(LCD_PIN_CS, LCD_PIN_DC, LCD_PIN_RESET, LCD_PIN_MOSI, LCD_PIN_SCLK, _theme),
    _engine(ANALOG1_PIN, ANALOG2_PIN),
    _buttons({BUTTON_PIN_LEFT, BUTTON_PIN_RIGHT, BUTTON_PIN_UP, BUTTON_PIN_DOWN}),
    _currentView(State::View::Invalid),
    _view(nullptr)
{
}

//! Initialize application.
void App::init() {
    _painter.begin();
    _painter.setRotation(LCD_ROTATION);
    _painter.fillScreen(BLACK);

    _engine.init();

    _buttonTimer.begin(&buttonISR, 1000);

    _state.mode = State::Mode::Scope;
    _state.scope.mode = State::Scope::Mode::Stacked;
    _state.level.mode = State::Level::Mode::Histogram;
    _state.navItem = State::NavItem::Param1;
}

//! Run application.
void App::run() {
    uint32_t lastTime = micros();
    int prevIndex = 0;
    while (true) {
        // change view if necessary
        changeView();
        // handle button presses
        for (int i = 0; i < 4; ++i) {
            while (_buttons[i].isPressed()) {
                if (_view) {
                    if (_view->handleKey(Key(i))) {
                        _view->drawNavigation();
                    }
                }
            }
        }
        if (_view) {
            // try to keep constant target framerate
            uint32_t currentTime = micros();
            uint32_t deltaTime = currentTime > lastTime ? (currentTime - lastTime) : (std::numeric_limits<uint32_t>::max() - lastTime + currentTime);
            lastTime = currentTime;
            if (deltaTime < _targetInterval) {
                delayMicroseconds(_targetInterval - deltaTime);
            }

            // process and redraw
            View::ProcessContext ctx;
            int writePos = _engine.writePos();
            ctx.begin = prevIndex;
            ctx.end = writePos;
            prevIndex = writePos;
            _view->process(ctx);
            _view->drawDynamic();
        }
    }
}

//! ISR level button scan.
void App::buttonTick() {
    for (auto &button : _buttons) {
        button.tick();
    }
}

//! Singleton.
App &App::instance() {
    static App *instance;
    if (!instance) {
        instance = new App();
    }
    return *instance;
}

void App::changeView() {
    if (_state.view() == _currentView) {
        return;
    }

    if (_view) {
        delete _view;
        _view = nullptr;
    }

    _currentView = _state.view();

    switch (_currentView) {
    case State::View::ScopeInput1:
        _view = new ViewScopeSingle(*this, 0); break;
    case State::View::ScopeInput2:
        _view = new ViewScopeSingle(*this, 1); break;
    case State::View::ScopeLayered:
        _view = new ViewScopeLayered(*this); break;
    case State::View::ScopeStacked:
        _view = new ViewScopeStacked(*this); break;
    case State::View::ScopeXY:
        _view = new ViewScopeXY(*this); break;
    case State::View::LevelHistogram:
        _view = new ViewLevelHistogram(*this); break;
    case State::View::LevelVU:
        _view = new ViewLevelVU(*this); break;
    case State::View::LevelMeter:
        _view = new ViewLevelMeter(*this); break;
    default:
        _view = new View(*this); break;
    }

    if (_view) {
        _view->drawNavigation();
        _view->drawStatic();
        _targetInterval = 1000000 / _view->targetFPS();
    }
}