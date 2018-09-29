#include "ViewLevelHistogram.h"



ViewLevelHistogram::ViewLevelHistogram(App &app) :
    ViewLevel(app)
{
    setParamCount(1);

    _engine.setSamplingInterval(10);

    for (uint32_t i = 0; i < HistogramBuffer::IntensityCount; ++i) {
        _colors[i] = _painter.Color565(0, i * 4, 0);
    }
}

ViewLevelHistogram::~ViewLevelHistogram() {
}

void ViewLevelHistogram::drawStatic() {
    clearContent();

    _painter.setFont(Painter::Font::Large);
    _painter.setTextColor(WHITE);
    _painter.printAligned(0, 0, 20, 160, Painter::HAlignment::Center, Painter::VAlignment::Center, "1");
    _painter.printAligned(128 - 20, 0, 20, 160, Painter::HAlignment::Center, Painter::VAlignment::Center, "2");

    for (int i = -1; i <= 1; i += 2) {
        int16_t x = 64 + i * 26;
        Painter::TickPosition tickPosition = i == -1 ? Painter::TickPosition::Left : Painter::TickPosition::Right;
        switch (_state.level.histogram.range) {
        case State::Range::Bipolar6V:
            _painter.drawVerticalScale(Point(x, 80), -6, 6, 10, 1, 2, tickPosition,
                [] (int16_t value, bool &drawTick, char *label) {
                    drawTick = true;
                    if (value % 5 == 0 || value == -6 || value == 6) {
                        sprintf(label, "%+d", value);
                    }
                }
            );
            break;
        case State::Range::Bipolar12V:
            _painter.drawVerticalScale(Point(x, 80), -12, 12, 5, 1, 2, tickPosition,
                [] (int16_t value, bool &drawTick, char *label) {
                    drawTick = true;
                    if (value % 5 == 0 || value == -12 || value == 12) {
                        sprintf(label, "%+d", value);
                    }
                }
            );
            break;
        case State::Range::Unipolar6V:
            _painter.drawVerticalScale(Point(x, 140), 0, 6, 20, 1, 2, tickPosition,
                [] (int16_t value, bool &drawTick, char *label) {
                    drawTick = true;
                    sprintf(label, "%+d", value);
                }
            );
            break;
        case State::Range::Unipolar12V:
            _painter.drawVerticalScale(Point(x, 140), 0, 12, 10, 1, 2, tickPosition,
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
}

void ViewLevelHistogram::drawDynamic() {
    for (uint32_t i = 0; i < HistogramBuffer::Bins; ++i) {
        int16_t y = 140 - i * 2;
        int16_t width = 20;
        int16_t offset = 2;
        _painter.drawFastHLine(64 - offset - width + 1, y, width, _colors[_histogramBuffer[0].intensity(i)]);
        _painter.drawFastHLine(64 + offset, y, width, _colors[_histogramBuffer[1].intensity(i)]);
        // _painter.drawFastHLine(64 - offset - width + 1, y, width, _colors[i % HistogramBuffer::IntensityCount]);
        // _painter.drawFastHLine(64 + offset, y, width, _colors[i % HistogramBuffer::IntensityCount]);
    }
}

void ViewLevelHistogram::process(const ProcessContext &ctx) {
    for (int i = 0; i < 2; ++i) {
        switch (_state.level.histogram.range) {
        case State::Range::Bipolar6V:
            _histogramBuffer[i].update(_engine.channel(i), ctx.begin, ctx.end, -6, 6);
            break;
        case State::Range::Bipolar12V:
            _histogramBuffer[i].update(_engine.channel(i), ctx.begin, ctx.end, -12, 12);
            break;
        case State::Range::Unipolar6V:
            _histogramBuffer[i].update(_engine.channel(i), ctx.begin, ctx.end, 0, 6);
            break;
        case State::Range::Unipolar12V:
            _histogramBuffer[i].update(_engine.channel(i), ctx.begin, ctx.end, 0, 12);
            break;
        default:
            break;
        }
    }
}

bool ViewLevelHistogram::handleKey(Key key) {
    if (_state.navItem == State::NavItem::Param0 && handleUpDown(_state.level.histogram.range, key)) {
        rangeChanged();
        return true;
    }

    return ViewLevel::handleKey(key);
}

void ViewLevelHistogram::drawFooter() {
    View::drawFooter();
    drawNavItem(ParamPos[2], Texts::Range[int(_state.level.histogram.range)], _state.navItem == State::NavItem::Param0);
}

void ViewLevelHistogram::rangeChanged() {
    drawStatic();
}
