#pragma once

#include "Common.h"

class MovingStats {
public:
    MovingStats(uint32_t log2Size) :
        _size(1 << log2Size),
        _mask((1 << log2Size) - 1),
        _index(0),
        _history(new int16_t[_size]),
        _sum(0)
    {
        for (uint32_t i = 0; i < _size; ++i) {
            _history[i] = 0;
        }
    }

    ~MovingStats() {
        delete [] _history;
    }

    inline void update(int16_t value) {
        // update history
        int16_t back = _history[_index];
        _index = (_index + 1) & _mask;
        _history[_index] = value;

        // update sum
        _sum -= back;
        _sum += value;
    }

    inline int16_t min() const {
        int16_t result = std::numeric_limits<int16_t>::max();
        for (uint32_t i = 0; i < _size; ++i) {
            result = std::min(result, _history[i]);
        }
        return result;
    }

    inline int16_t max() const {
        int16_t result = std::numeric_limits<int16_t>::lowest();
        for (uint32_t i = 0; i < _size; ++i) {
            result = std::max(result, _history[i]);
        }
        return result;
    }

    inline int16_t avg() {
        return _sum / _size;
    }

private:
    uint32_t _size;
    uint32_t _mask;
    uint32_t _index;
    int16_t *_history;
    int32_t _sum;
};
