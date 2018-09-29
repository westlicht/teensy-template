#pragma once

#include <cstdint>

//! 2D point.
struct Point {
    int16_t x;
    int16_t y;
    constexpr Point(int16_t x_, int16_t y_) : x(x_), y(y_) {}
};

typedef Point Size;