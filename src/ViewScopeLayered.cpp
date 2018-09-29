#include "ViewScopeLayered.h"

ViewScopeLayered::ViewScopeLayered(App &app) :
    ViewScope(app)
{
}

ViewScopeLayered::~ViewScopeLayered() {
}

void ViewScopeLayered::drawStatic() {
    clearContent();
    drawFullScale();
    drawFullGrid();
}

void ViewScopeLayered::drawDynamic() {
    for (int i = 0; i < 2; ++i) {
        switch (_state.scope.range) {
        case State::Range::Bipolar6V:
            _waveformBuffer[i].update(_engine.channel(i), _end, -6, 6, 10, 1, 80);
            break;
        case State::Range::Bipolar12V:
            _waveformBuffer[i].update(_engine.channel(i), _end, -12, 12, 5, 1, 80);
            break;
        case State::Range::Unipolar6V:
            _waveformBuffer[i].update(_engine.channel(i), _end, 0, 6, 20, 1, 140);
            break;
        case State::Range::Unipolar12V:
            _waveformBuffer[i].update(_engine.channel(i), _end, 0, 12, 10, 1, 140);
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
            drawFullGridPart(x, y00, y01);
            drawFullGridPart(x, y10, y11);
            // drawFullGridPart(x, std::min(y00, y10), std::max(y01, y11));
        } else {
            // if (y01 < y10 || y00 > y11) {
                _painter.drawFastVLine(x, y00, y01 - y00, _theme.traceLayered0);
                _painter.drawFastVLine(x, y10, y11 - y10, _theme.traceLayered1);
            // } else {
            //     if (y00 < y10) {
            //         _painter.drawFastVLine(x, y00, y10 - y00, _theme.traceLayered0);
            //         _painter.drawFastVLine(x, y10, y01 - y10, _theme.traceLayeredBoth);
            //         _painter.drawFastVLine(x, y01, y11 - y01, _theme.traceLayered1);
            //     } else {
            //         _painter.drawFastVLine(x, y11, y01 - y11, _theme.traceLayered0);
            //         _painter.drawFastVLine(x, y00, y11 - y00, _theme.traceLayeredBoth);
            //         _painter.drawFastVLine(x, y10, y00 - y10, _theme.traceLayered1);
            //     }
            // }
        }
    };

    for (int i = 0; i < WaveformBuffer::Size; ++i) {
        drawSegment(16 + i, _waveformBuffer[0].last(i), _waveformBuffer[0].last(i + 1), _waveformBuffer[1].last(i), _waveformBuffer[1].last(i + 1), true);
        drawSegment(16 + i, _waveformBuffer[0].current(i), _waveformBuffer[0].current(i + 1), _waveformBuffer[1].current(i), _waveformBuffer[1].current(i + 1), false);
    }
}

void ViewScopeLayered::process(const ProcessContext &ctx) {
    _end = ctx.end;
}
