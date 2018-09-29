#pragma once

#include "Common.h"

class Channel {
public:
    Channel();
    ~Channel();

    inline int writePos() const { return _writePos; }
    inline int wrap(int index) const { return (index + Size) & Mask; }

    inline bool isEmpty() const {
        return _writePos == _readPos;
    }

    inline bool isFull() const {
        return (_writePos == (_readPos ^ Size));
    }

    inline void write(int16_t sample) {
        _samples[_writePos] = sample;
        if (isFull()) {
            advance(_readPos);
        }
        advance(_writePos);
    }

    inline int16_t read(int index) const {
        return _samples[wrap(index)];
    }

    inline int16_t read() {
        int16_t sample = _samples[_readPos];
        advance(_readPos);
        return sample;
    }

private:
    inline void advance(volatile int &pos) const {
        pos = (pos + 1) & Mask;
    }

    static const int Log2Size = 12;
    static const int Size = 1<<Log2Size;
    static const int Mask = (1<<Log2Size) - 1;

    volatile int _writePos;
    volatile int _readPos;

    int16_t _samples[Size];
};
