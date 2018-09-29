#include "Painter.h"
#include "Voltage.h"

#include "fonts/small_numbers.c"

#include "_fonts/akashi20.c"
#include "_fonts/arial20.c"
#include "_fonts/titlingCaps.c"
#include "_fonts/akasha_cap.c"
#include "_fonts/orbitron14.c"

Painter::Painter(const uint8_t cspin, const uint8_t dcpin, const uint8_t rstpin, const uint8_t mosi, const uint8_t sclk, const Theme &theme) :
    TFT_ST7735(cspin, dcpin, rstpin, mosi, sclk),
    _theme(theme)
{}

// Painter::Painter(TFT_ST7735 &tft, const Theme &theme) :
//     _tft(tft),
//     _theme(theme)
// {
// }

void Painter::setFont(Painter::Font font) {
    switch (font) {
    case Font::Default:
        TFT_ST7735::setFont(nullptr);
        break;
    case Font::Small:
        TFT_ST7735::setFont(&small_numbers);
        break;
    case Font::Large:
        // TFT_ST7735::setFont(&akashi20);
        // TFT_ST7735::setFont(&arial20);
        // TFT_ST7735::setFont(&titlingCaps);
        // TFT_ST7735::setFont(&akasha_cap);
        TFT_ST7735::setFont(&orbitron14);

        break;
    }
}

void Painter::printAligned(int16_t x, int16_t y, int16_t w, int16_t h, HAlignment hAlignment, VAlignment vAlignment, const char *text) {
    int16_t cx = x;
    int16_t cy = y;

    switch (hAlignment) {
    case HAlignment::Left:      cx = x; break;
    case HAlignment::Right:     cx = x + w - textWidth(text); break;
    case HAlignment::Center:    cx = x + (w - textWidth(text)) / 2; break;
    }
    
    switch (vAlignment) {
    case VAlignment::Top:       cy = y; break;
    case VAlignment::Bottom:    cy = y + h - (_currentFont->font_height/* - _currentFont->font_descent*/); break;
    case VAlignment::Center:    cy = y + (h - _currentFont->font_height) / 2; break;
    }
    
    setCursor(cx, cy);
    print(text);
}

void Painter::printAt(int16_t x, int16_t y, voltage_t voltage) {
    setCursor(x, y);
    printf("%+d.%03d", voltage / Voltage::StepsPerVolt, (voltage % Voltage::StepsPerVolt) * Voltage::MillivoltsPerStep);
}
