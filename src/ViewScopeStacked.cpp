#include "ViewScopeStacked.h"

ViewScopeStacked::ViewScopeStacked(App &app) :
    ViewScope(app)
{
}

ViewScopeStacked::~ViewScopeStacked() {
}

void ViewScopeStacked::drawStatic() {
    ViewScope::drawStatic();
    drawStackedScale();
    drawStackedGrid();
}

void ViewScopeStacked::drawDynamic() {
    for (int i = 0; i < 2; ++i) {
        switch (_state.scope.range) {
        case State::Range::Bipolar6V:
            _waveformBuffer[i].update(_engine.channel(i), _end, -6, 6, 5, 1, 80 + (i * 2 - 1) * 30);
            break;
        case State::Range::Bipolar12V:
            _waveformBuffer[i].update(_engine.channel(i), _end, -12, 12, 5, 2, 80 + (i * 2 - 1) * 30);
            break;
        case State::Range::Unipolar6V:
            _waveformBuffer[i].update(_engine.channel(i), _end, 0, 6, 10, 1, 80 + i * 60);
            break;
        case State::Range::Unipolar12V:
            _waveformBuffer[i].update(_engine.channel(i), _end, 0, 12, 5, 1, 80 + i * 60);
            break;
        default:
            break;
        }
    }

    auto drawSegment = [&] (int16_t x, int16_t y00, int16_t y01, int16_t y10, int16_t y11, bool clear) {
        if (y00 > y01) {
            std::swap(y00, y01);
        }
        if (y10 > y11) {
            std::swap(y10, y11);
        }
        if (clear) {
            drawStackedGridPart(x, y00, y01);
            drawStackedGridPart(x, y10, y11);
        } else {
            _painter.drawFastVLine(x, y00, y01 - y00, _theme.traceSingle);
            _painter.drawFastVLine(x, y10, y11 - y10, _theme.traceSingle);
        }
    };

    for (int i = 0; i < WaveformBuffer::Size; ++i) {
        drawSegment(16 + i, _waveformBuffer[0].last(i), _waveformBuffer[0].last(i + 1), _waveformBuffer[1].last(i), _waveformBuffer[1].last(i + 1), true);
        drawSegment(16 + i, _waveformBuffer[0].current(i), _waveformBuffer[0].current(i + 1), _waveformBuffer[1].current(i), _waveformBuffer[1].current(i + 1), false);
    }
}

void ViewScopeStacked::process(const ProcessContext &ctx) {
    _end = ctx.end;
}
