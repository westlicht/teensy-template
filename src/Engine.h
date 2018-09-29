#pragma once

#include "Common.h"
#include "Channel.h"

#include <ADC.h>
#include <IntervalTimer.h>

class Engine {
public:
    Engine(uint8_t pin0, uint8_t pin1);

    const Channel &channel(int index) const { return _channels[index]; }
          Channel &channel(int index)       { return _channels[index]; }

    void init();

    void setSamplingInterval(int usec);

    void updateTick();
    //void setTriggerLevels();

    inline int writePos() const { return _channels[0].writePos(); }
    inline int wrap(int index) const { return _channels[0].wrap(index); }

private:
    void enableInterrupts();
    void disableInterrupts();


    uint8_t _pin0;
    uint8_t _pin1;
    Channel _channels[2];

    ADC _adc;
    IntervalTimer _timer;
};
