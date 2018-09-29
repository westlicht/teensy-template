#pragma once

#include "Common.h"

class HistogramBuffer {
public:
    static constexpr uint32_t Bins = 61;
    static constexpr uint32_t HistoryLog2Size = 14;
    static constexpr uint32_t HistorySize = 1 << HistoryLog2Size;
    static constexpr uint32_t HistoryMask = HistorySize - 1;
    static constexpr uint32_t IntensityCount = 64;

    HistogramBuffer()
    {
        memset(_bins, 0, sizeof(_bins));
        memset(_history, 0xff, sizeof(_history));
        _historyIndex = 0;
        _max = 1;
    }

    void update(const Channel &channel, int begin, int end, int16_t lowV, int16_t highV) {

        int16_t low = Voltage::voltsToRaw(lowV);
        int16_t high = Voltage::voltsToRaw(highV);
        int range = high - low;

#if 0
        memset(_bins, 0, sizeof(_bins));
        int value = std::max(low, std::min(high, channel.read(begin)));
        int bin = ((value - low) * Bins) / range;
        bin = std::max(0, std::min(int(Bins) - 1, bin));
        _bins[bin] = 1;
        _max = 1;
        return;
#endif

        for (int index = begin; index != end; index = channel.wrap(index + 1)) {
            int value = std::max(low, std::min(high, channel.read(index)));
            int bin = ((value - low) * Bins) / range;
            ++_bins[bin];

            uint8_t &historyItem = _history[_historyIndex];
            if (historyItem != 0xff) {
                --_bins[historyItem];
            }
            historyItem = bin;
            _historyIndex = (_historyIndex + 1) & HistoryMask;
        }

        _max = 0;
        for (uint32_t bin = 0; bin < Bins; ++bin) {
            _max = std::max(_max, _bins[bin]);
        }
    }

    int16_t bin(int index) const { return _bins[index]; }

    int16_t intensity(int index) const {
        return (_bins[index] * IntensityCount) / _max;
    }

private:
    int _bins[Bins];
    int _max;
    uint8_t _history[HistorySize];
    int _historyIndex;
};
