#pragma once

#include "Common.h"
#include "Point.h"
#include "Theme.h"
#include "Voltage.h"

class Painter : public TFT_ST7735 {
public:
    Painter(const uint8_t cspin, const uint8_t dcpin, const uint8_t rstpin, const uint8_t mosi, const uint8_t sclk, const Theme &theme);

    enum class Font { Default, Small, Large };
    enum class HAlignment { Left, Right, Center };
    enum class VAlignment { Top, Bottom, Center };

    void setFont(Font font);

    int16_t textWidth(const char *text) { return getTextWidth(text); }
    int16_t textHeight() { return _currentFont->font_height - _currentFont->font_descent; }

    void printAligned(int16_t x, int16_t y, int16_t w, int16_t h, HAlignment hAlignment, VAlignment vAlignment, const char *text);

    void printAt(int16_t x, int16_t y, const char *text) {
        setCursor(x, y);
        print(text);
    }

    void printAt(const Point &pos, const char *text) {
        setCursor(pos.x, pos.y);
        print(text);
    }

    void printAt(int16_t x, int16_t y, voltage_t voltage);

    enum class TickPosition { Left, Right, Top, Bottom };

    static constexpr int16_t VerticalLabelOffset = -2;

    // tickSize(int16_t value, bool &drawTick, char label[8])
    template<typename TickInfo>
    void drawVerticalScale(
        const Point &origin,
        int16_t rangeMin, int16_t rangeMax,
        int16_t stepSize,
        int16_t smallTickSize,
        int16_t largeTickSize,
        TickPosition tickPosition,
        TickInfo tickInfo
    ) {
        for (int16_t value = rangeMin; value <= rangeMax; ++value) {
            int16_t y = origin.y - value * stepSize;
            char label[8]; label[0] = '\0';
            bool drawTick = true;
            tickInfo(value, drawTick, label);
            if (drawTick) {
                if (label[0] != '\0') {
                    setFont(Font::Small);
                    setTextColor(_theme.scaleLabel);
                    if (tickPosition == TickPosition::Left) {
                        // ticks to the left
                        setCursor(origin.x - largeTickSize - 1 - getTextWidth(label), y + VerticalLabelOffset);
                        drawFastHLine(origin.x - largeTickSize, y, largeTickSize, _theme.scaleTick);
                    } else if (tickPosition == TickPosition::Right) {
                        // ticks to the right
                        setCursor(origin.x + largeTickSize + 2, y + VerticalLabelOffset);
                        drawFastHLine(origin.x + 1, y, largeTickSize, _theme.scaleTick);
                    }
                    TFT_ST7735::print(label);
                } else {
                    if (tickPosition == TickPosition::Left) {
                        // ticks to the left
                        drawFastHLine(origin.x - smallTickSize, y, smallTickSize, _theme.scaleTick);
                    } else if (tickPosition == TickPosition::Right) {
                        // ticks to the right
                        drawFastHLine(origin.x + 1, y, smallTickSize, _theme.scaleTick);
                    }
                }
            }
        }

        drawFastVLine(origin.x, origin.y - rangeMax * stepSize, (rangeMax - rangeMin) * stepSize + 1, _theme.scale);
    }

    // tickSize(int16_t value, bool &drawTick, char label[8])
    template<typename TickInfo>
    void drawHorizontalScale(
        const Point &origin,
        int16_t rangeMin, int16_t rangeMax,
        int16_t stepSize,
        int16_t smallTickSize,
        int16_t largeTickSize,
        TickPosition tickPosition,
        TickInfo tickInfo
    ) {
        for (int16_t value = rangeMin; value <= rangeMax; ++value) {
            int16_t x = origin.x + value * stepSize;
            char label[8]; label[0] = '\0';
            bool drawTick = true;
            tickInfo(value, drawTick, label);
            if (drawTick) {
                if (label[0] != '\0') {
                    setFont(Font::Small);
                    setTextColor(_theme.scaleLabel);
                    if (tickPosition == TickPosition::Top) {
                        // ticks to the top
                        setCursor(x - (getTextWidth(label) - 1) / 2, origin.y - largeTickSize - 6);
                        drawFastVLine(x, origin.y - largeTickSize, largeTickSize, _theme.scaleTick);
                    } else if (tickPosition == TickPosition::Bottom) {
                        // ticks to the bottom
                        setCursor(x - (getTextWidth(label) - 1) / 2, origin.y + largeTickSize + 2);
                        drawFastVLine(x, origin.y + 1, largeTickSize, _theme.scaleTick);
                    }
                    TFT_ST7735::print(label);
                } else {
                    if (tickPosition == TickPosition::Top) {
                        // ticks to the top
                        drawFastVLine(x, origin.y - smallTickSize, smallTickSize, _theme.scaleTick);
                    } else if (tickPosition == TickPosition::Bottom) {
                        // ticks to the bottom
                        drawFastVLine(x, origin.y + 1, smallTickSize, _theme.scaleTick);
                    }
                }
            }
        }

        drawFastHLine(origin.x + rangeMin * stepSize, origin.y, (rangeMax - rangeMin) * stepSize + 1, _theme.scale);
    }

private:
    const Theme &_theme;
};
