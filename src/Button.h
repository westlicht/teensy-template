#pragma once

#include "Common.h"

//! Button debouncing.
class Button {
public:
    Button(uint8_t pin);

    void tick();

    bool isDown() const;
    bool isPressed() const;

private:
    static constexpr uint32_t DebounceTicks = 5;

    uint8_t _pin;

    uint8_t _state;
    uint8_t _ticks;

    volatile uint8_t _down;
    mutable volatile uint8_t _pressed;
};
