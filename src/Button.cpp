#include "Button.h"

//! Constructor.
Button::Button(uint8_t pin) :
    _pin(pin),
    _state(1),
    _ticks(0),
    _down(0),
    _pressed(0)
{
    pinMode(_pin, INPUT_PULLUP);
}

//! ISR level!
//! Called every tick to update the internal state.
void Button::tick() {

    uint8_t state = digitalRead(_pin);
    if (state != _state) {
        _state = state;
        _ticks = 0;
    } else {
        if (++_ticks == DebounceTicks) {
            _down = !_state;
            if (_down) {
                ++_pressed;
            }
        } else if (_ticks > DebounceTicks) {
            _ticks = DebounceTicks + 1;
        }
    }
}

//! Return true if button is currently down.
bool Button::isDown() const {
    return _down;
}

//! Return true if button was pressed recently.
bool Button::isPressed() const {
    if (_pressed > 0) {
        --_pressed;
        return true;
    } else {
        return false;
    }
}
