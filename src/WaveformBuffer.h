#pragma once

#include "Common.h"

class WaveformBuffer {
public:
    static constexpr int Size = 112;

    WaveformBuffer() :
        _initialized(false),
        _currentBuf(_buf[0]),
        _lastBuf(_buf[1])
    {
        memset(_buf, 0, sizeof(_buf));
    }

    void update(const Channel &channel, int index, int16_t lowV, int16_t highV, int16_t pixelsPerVoltNom, int16_t pixelsPerVoltDenom, int16_t y) {
        std::swap(_currentBuf, _lastBuf);

        int16_t low = Voltage::voltsToRaw(lowV);
        int16_t high = Voltage::voltsToRaw(highV);
        int16_t divider = Voltage::voltsToRaw(1) * pixelsPerVoltDenom / pixelsPerVoltNom;

        for (int i = 0; i <= Size; ++i) {
            int16_t value = std::max(low, std::min(high, channel.read(index - Size + i - 1))) / divider;
            _currentBuf[i] = y - value;
        }

        if (!_initialized) {
            memcpy(_lastBuf, _currentBuf, sizeof(int16_t) * (Size + 1));
            _initialized = true;
        }
    }

    int16_t current(int index) const { return _currentBuf[index]; }
    int16_t last(int index) const { return _lastBuf[index]; }

private:
    bool _initialized;
    int16_t _buf[2][Size + 1];
    int16_t *_currentBuf;
    int16_t *_lastBuf;
};
