#pragma once

#include "Print.h"
#include "Painter.h"

class TextWidget : public Print {
public:
    TextWidget(const Point &pos, const Size &size,
               Painter::HAlignment hAlignment, Painter::VAlignment vAlignment,
               Painter::Font font,
               uint16_t foreground, uint16_t background) :
        _pos(pos),
        _size(size),
        _hAlignment(hAlignment),
        _vAlignment(vAlignment),
        _font(font),
        _foreground(foreground),
        _background(background),
        _dirty(true)
    {
        _text[0] = '\0';
    }

    void draw(Painter &painter, bool force = false) {
        if (_dirty || force) {
            painter.setFont(_font);
            painter.setTextColor(_foreground, _background);
            painter.printAligned(_pos.x, _pos.y, _size.x, _size.y, _hAlignment, _vAlignment, _text);
            _dirty = false;
        }
    }

protected:
    virtual size_t write(uint8_t b) override {
        return 0;    
    }

private:
    Point _pos;
    Size _size;
    Painter::HAlignment _hAlignment;
    Painter::VAlignment _vAlignment;
    Painter::Font _font;
    uint16_t _foreground;
    uint16_t _background;
    char _text[16];
    bool _dirty;
};
