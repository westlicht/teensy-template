#include "ViewScope.h"

ViewScope::ViewScope(App &app) : View(app) {
    setParamCount(2);
    timeBaseChanged();
}

ViewScope::~ViewScope() {
}

void ViewScope::drawStatic() {
    clearContent();
}

#if 0
void ViewScope::update() {
    auto drawSegment = [&] (int16_t x, int16_t y0, int16_t y1, uint16_t color) {
        // _painter.drawPixel(x, 80 - y0, color);
        // return;
        if (y0 >= y1) {
            _painter.drawFastVLine(x, 80 - y0, y0 - y1, color);
        } else {
            _painter.drawFastVLine(x, 80 - y1, y1 - y0, color);
        }
    };

    memcpy(_lastBuf, _currentBuf, sizeof(_lastBuf));
    const auto &channel = _app.engine().channel(0);
    int writePos = channel.writePos();
    for (int i = 0; i < 128; ++i) {
        _currentBuf[i] = channel.read(writePos - 128 + i) / 1000;
    }
    _phase -= 0.01f;

    for (int i = 0; i < 128 - 16; ++i) {
        drawSegment(16 + i, _lastBuf[i], _lastBuf[i + 1], BLACK);
        drawSegment(16 + i, _currentBuf[i], _currentBuf[i + 1], WHITE);
    }
}
#endif

//! Key handler.
//! - handles submode selection with up/down keys
bool ViewScope::handleKey(Key key) {
    if (View::handleKey(key)) {
        return true;
    }
    if (_state.navItem == State::NavItem::SubMode && handleUpDown(_state.scope.mode, key)) {
        return true;
    }
    if (_state.navItem == State::NavItem::Param0 && handleUpDown(_state.scope.timeBase, key)) {
        timeBaseChanged();
        return true;
    }
    if (_state.navItem == State::NavItem::Param1 && handleUpDown(_state.scope.range, key)) {
        rangeChanged();
        return true;
    }
    return false;
}

void ViewScope::drawHeader() {
    View::drawHeader();
    drawNavItem(SubModePos, Texts::Scope::Mode[int(_state.scope.mode)], _state.navItem == State::NavItem::SubMode);
}

void ViewScope::drawFooter() {
    View::drawFooter();
    drawNavItem(ParamPos[0], Texts::TimeBase[int(_state.scope.timeBase)], _state.navItem == State::NavItem::Param0);
    drawNavItem(ParamPos[2], Texts::Range[int(_state.scope.range)], _state.navItem == State::NavItem::Param1);
}

void ViewScope::drawFullScale() {
    switch (_state.scope.range) {
    case State::Range::Bipolar6V:
        _painter.drawVerticalScale(Point(15, 80), -6, 6, 10, 1, 2, Painter::TickPosition::Left,
            [] (int16_t value, bool &drawTick, char *label) {
                drawTick = true;
                if (value % 5 == 0 || value == -6 || value == 6) {
                    sprintf(label, "%+d", value);
                }
            }
        );
        break;
    case State::Range::Bipolar12V:
        _painter.drawVerticalScale(Point(15, 80), -12, 12, 5, 1, 2, Painter::TickPosition::Left,
            [] (int16_t value, bool &drawTick, char *label) {
                drawTick = true;
                if (value % 5 == 0 || value == -12 || value == 12) {
                    sprintf(label, "%+d", value);
                }
            }
        );
        break;
    case State::Range::Unipolar6V:
        _painter.drawVerticalScale(Point(15, 140), 0, 6, 20, 1, 2, Painter::TickPosition::Left,
            [] (int16_t value, bool &drawTick, char *label) {
                drawTick = true;
                sprintf(label, "%+d", value);
            }
        );
        break;
    case State::Range::Unipolar12V:
        _painter.drawVerticalScale(Point(15, 140), 0, 12, 10, 1, 2, Painter::TickPosition::Left,
            [] (int16_t value, bool &drawTick, char *label) {
                drawTick = true;
                if (value % 2 == 0) {
                    sprintf(label, "%+d", value);
                }
            }
        );
        break;
    default:
        break;
    }
}

void ViewScope::drawFullGrid() {
    switch (_state.scope.range) {
    case State::Range::Bipolar6V:
    case State::Range::Bipolar12V:
        for (int i = 1; i <= TimeBaseCount; ++i) {
            int16_t x = 16 + i * TimeBaseWidth;
            _painter.drawFastVLine(x, 20, 121, _theme.grid);
        }
        for (int i = -12; i <= 12; ++i) {
            if (i % 5 == 0 || i == -12 || i == 12) {
                int16_t y = 80 + i * 5;
                _painter.drawFastHLine(16, y, 128 - 16, _theme.grid);

            }
        }
        break;
    case State::Range::Unipolar6V:
    case State::Range::Unipolar12V:
        for (int i = 1; i <= TimeBaseCount; ++i) {
            int16_t x = 16 + i * TimeBaseWidth;
            _painter.drawFastVLine(x, 20, 121, _theme.grid);
        }
        for (int i = 0; i <= 6; ++i) {
            int16_t y = 140 - i * 20;
            _painter.drawFastHLine(16, y, 128 - 16, _theme.grid);
        }
        break;
    default:
        break;
    }
}

void ViewScope::drawFullGridPart(int16_t x, int16_t y0, int16_t y1) {
    if ((x - 16) % TimeBaseWidth == 0) {
        _painter.drawFastVLine(x, y0, y1 - y0, _theme.grid);
    } else {
        _painter.drawFastVLine(x, y0, y1 - y0, _theme.background);
        switch (_state.scope.range) {
        case State::Range::Bipolar6V:
        case State::Range::Bipolar12V:
            for (int i = -12; i <= 12; ++i) {
                if (i % 5 == 0 || i == -12 || i == 12) {
                    int16_t y = 80 + i * 5;
                    _painter.drawPixel(x, y, _theme.grid);

                }
            }
            break;
        case State::Range::Unipolar6V:
        case State::Range::Unipolar12V:
            for (int i = 0; i <= 6; ++i) {
                int16_t y = 140 - i * 20;
                _painter.drawPixel(x, y, _theme.grid);
            }
            break;
        default:
            break;
        }
    }
}

void ViewScope::drawStackedScale() {
    for (int i = 0; i < 2; ++i) {
        switch (_state.scope.range) {
        case State::Range::Bipolar6V:
            _painter.drawVerticalScale(Point(15, 80 + (i * 2 - 1) * 30), -6, 6, 5, 1, 2, Painter::TickPosition::Left,
                [] (int16_t value, bool &drawTick, char *label) {
                    drawTick = true;
                    if (value % 5 == 0) {
                        sprintf(label, "%+d", value);
                    }
                }
            );
            break;
        case State::Range::Bipolar12V:
            _painter.drawVerticalScale(Point(15, 80 + (i * 2 - 1) * 30), -6, 6, 5, 1, 2, Painter::TickPosition::Left,
                [] (int16_t value, bool &drawTick, char *label) {
                    drawTick = true;
                    if (value % 5 == 0) {
                        sprintf(label, "%+d", value * 2);
                    }
                }
            );
            break;
        case State::Range::Unipolar6V:
            _painter.drawVerticalScale(Point(15, 80 + i * 60), 0, 6, 10, 1, 2, Painter::TickPosition::Left,
                [i] (int16_t value, bool &drawTick, char *label) {
                    drawTick = true;
                    if (value % 2 == 0 && (i != 0 || value != 6)) {
                        sprintf(label, "%+d", value);
                    }
                }
            );
            break;
        case State::Range::Unipolar12V:
            _painter.drawVerticalScale(Point(15, 80 + i * 60), 0, 6, 10, 1, 2, Painter::TickPosition::Left,
                [i] (int16_t value, bool &drawTick, char *label) {
                    drawTick = true;
                    if (value % 2 == 0 && (i != 0 || value != 6)) {
                        sprintf(label, "%+d", 2 * value);
                    }
                }
            );
            break;
        default:
            break;
        }
    }
}

void ViewScope::drawStackedGrid() {
    switch (_state.scope.range) {
    case State::Range::Bipolar6V:
    case State::Range::Bipolar12V:
        for (int i = 1; i <= TimeBaseCount; ++i) {
            int16_t x = 16 + i * TimeBaseWidth;
            _painter.drawFastVLine(x, 20, 121, _theme.grid);
        }
        for (int i = 0; i < 2; ++i) {
            for (int j = -6; j <= 6; ++j) {
                if (j % 5 == 0 || j == -6 || j == 6) {
                    int16_t y = 80 + (i * 2 - 1) * 30 + j * 5;
                    _painter.drawFastHLine(16, y, 128 - 16, _theme.grid);
                }
            }
        }
        break;
    case State::Range::Unipolar6V:
    case State::Range::Unipolar12V:
        for (int i = 1; i <= TimeBaseCount; ++i) {
            int16_t x = 16 + i * TimeBaseWidth;
            _painter.drawFastVLine(x, 20, 121, _theme.grid);
        }
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j <= 3; ++j) {
                int16_t y = 80 + i * 60 - j * 20;
                _painter.drawFastHLine(16, y, 128 - 16, _theme.grid);
            }
        }
        break;
    default:
        break;
    }
}

void ViewScope::drawStackedGridPart(int16_t x, int16_t y0, int16_t y1) {
    if ((x - 16) % TimeBaseWidth == 0) {
        _painter.drawFastVLine(x, y0, y1 - y0, _theme.grid);
    } else {
        _painter.drawFastVLine(x, y0, y1 - y0, _theme.background);
        switch (_state.scope.range) {
        case State::Range::Bipolar6V:
        case State::Range::Bipolar12V:
            for (int i = 0; i < 2; ++i) {
                for (int j = -6; j <= 6; ++j) {
                    if (j % 5 == 0 || j == -6 || j == 6) {
                        int16_t y = 80 + (i * 2 - 1) * 30 + j * 5;
                        _painter.drawPixel(x, y, _theme.grid);
                    }
                }
            }
            break;
        case State::Range::Unipolar6V:
        case State::Range::Unipolar12V:
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j <= 3; ++j) {
                    int16_t y = 80 + i * 60 - j * 20;
                    _painter.drawPixel(x, y, _theme.grid);
                }
            }
            break;
        default:
            break;
        }
    }
}

void ViewScope::timeBaseChanged() {
    int timeBase = 100;
    switch (_state.scope.timeBase) {
    case State::TimeBase::T100us:    timeBase = 100;     break;
    case State::TimeBase::T250us:    timeBase = 250;     break;
    case State::TimeBase::T500us:    timeBase = 500;     break;
    case State::TimeBase::T1ms:      timeBase = 1000;    break;
    case State::TimeBase::T2_5ms:    timeBase = 2500;    break;
    case State::TimeBase::T5ms:      timeBase = 5000;    break;
    case State::TimeBase::T10ms:     timeBase = 10000;   break;
    case State::TimeBase::T25ms:     timeBase = 25000;   break;
    case State::TimeBase::T50ms:     timeBase = 50000;   break;
    case State::TimeBase::T100ms:    timeBase = 100000;  break;
    case State::TimeBase::T250ms:    timeBase = 250000;  break;
    case State::TimeBase::T500ms:    timeBase = 500000;  break;
    case State::TimeBase::T1s:       timeBase = 1000000; break;
    case State::TimeBase::T2_5s:     timeBase = 2500000; break;
    case State::TimeBase::T5s:       timeBase = 5000000; break;
    default: break;
    }
    _app.engine().setSamplingInterval(timeBase / TimeBaseWidth);
}

void ViewScope::rangeChanged() {
    drawStatic();
}
