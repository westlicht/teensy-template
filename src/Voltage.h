#pragma once

#include "Common.h"

typedef int16_t voltage_t;

class Voltage {
public:
    // millivolts/step  = 0.5
    // volts/step       = 0.0005
    // steps/millivolt  = 2
    // steps/volt       = 2000

    static constexpr int32_t Scaling = 2000; // Volts -> Raw scaling
    static constexpr int16_t MillivoltsPerStep = 5;
    static constexpr int16_t StepsPerVolt = 1000 / MillivoltsPerStep;

    static inline int16_t rawToVolts(int16_t raw) {
        return raw / 2000;
    }

    static inline int16_t rawToMilliVolts(int16_t raw) {
        return raw / 2;
    }

    static inline int16_t voltsToRaw(int16_t V) {
        return V * 2000; 
    }

    static inline int16_t milliVoltsToRaw(int16_t mV) {
        return mV * 2;
    }

    static void print(char *buf, size_t len, voltage_t v) {
        snprintf(buf, len, "%c%d.%03d", v >= 0 ? '+' : '-', v / StepsPerVolt, (v % StepsPerVolt) * MillivoltsPerStep);
    }
};
