#include "ViewScopeSingle.h"

ViewScopeSingle::ViewScopeSingle(App &app, int channel) :
    ViewScope(app),
    _channel(channel)
{
}

ViewScopeSingle::~ViewScopeSingle() {
}

void ViewScopeSingle::drawStatic() {
    clearContent();
    drawFullScale();
    drawFullGrid();
}

void ViewScopeSingle::drawDynamic() {
    switch (_state.scope.range) {
    case State::Range::Bipolar6V:
        _waveformBuffer.update(_engine.channel(_channel), _end, -6, 6, 10, 1, 80);
        break;
    case State::Range::Bipolar12V:
        _waveformBuffer.update(_engine.channel(_channel), _end, -12, 12, 5, 1, 80);
        break;
    case State::Range::Unipolar6V:
        _waveformBuffer.update(_engine.channel(_channel), _end, 0, 6, 20, 1, 140);
        break;
    case State::Range::Unipolar12V:
        _waveformBuffer.update(_engine.channel(_channel), _end, 0, 12, 10, 1, 140);
        break;
    default:
        break;
    }

    auto drawSegment = [&] (int16_t x, int16_t y0, int16_t y1, bool clear) {
        if (y0 > y1) {
            std::swap(y0, y1);
        }
        if (clear) {
            drawFullGridPart(x, y0, y1);
        } else {
            _painter.drawFastVLine(x, y0, y1 - y0, _theme.traceSingle);
        }
    };

    for (int i = 0; i < WaveformBuffer::Size; ++i) {
        drawSegment(16 + i, _waveformBuffer.last(i), _waveformBuffer.last(i + 1), true);
        drawSegment(16 + i, _waveformBuffer.current(i), _waveformBuffer.current(i + 1), false);
    }
}

void ViewScopeSingle::process(const ProcessContext &ctx) {
    _end = ctx.end;
}
