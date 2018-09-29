#include "Engine.h"
#include "Voltage.h"

#define ADC_RESOLUTION          16
#define ADC_CONVERSION_SPEED    ADC_HIGH_SPEED
#define ADC_SAMPLING_SPEED      ADC_HIGH_SPEED
#define ADC_AVERAGING           0

#define SIMULATE

struct Waveform {
    static constexpr int Scale = 1;

    int frequency;  // hz
    int lo;         // raw value
    int hi;         // raw value

    int value;
    int range;
    int increment;

    Waveform(int frequency_, int lo_, int hi_) :
        frequency(frequency_),
        lo(Voltage::milliVoltsToRaw(lo_) * Scale),
        hi(Voltage::milliVoltsToRaw(hi_) * Scale)
    {}

    void reset(int interval) {
        value = lo;
        range = (hi - lo);
        increment = (int64_t(range) * frequency * interval) / 1000000ll;
    }

    int16_t process() {
        value += increment;
        while (value > hi) {
            value -= range;
        }
        return value / Scale;
    }
};

#ifdef SIMULATE
static Waveform waveform0(1000, -5000, 5000);
static Waveform waveform1(1, -5000, 5000);
#endif

static Engine *g_engine = nullptr;

void adc0_isr() {    
}

void adc1_isr() {
}

static void sampleISR() {
    if (g_engine) {
        g_engine->updateTick();
    }
}

Engine::Engine(uint8_t pin0, uint8_t pin1) :
    _pin0(pin0),
    _pin1(pin1)
{
    setSamplingInterval(10);

    g_engine = this;
}

void Engine::init() {
    pinMode(_pin0, INPUT);
    pinMode(_pin1, INPUT);

    _adc.setReference(ADC_REF_3V3);
    // _adc.setReference(ADC_REF_EXT);

    _adc.setResolution(ADC_RESOLUTION, ADC_0);
    _adc.setResolution(ADC_RESOLUTION, ADC_1);

    _adc.setConversionSpeed(ADC_CONVERSION_SPEED, ADC_0);
    _adc.setConversionSpeed(ADC_CONVERSION_SPEED, ADC_1);

    _adc.setSamplingSpeed(ADC_SAMPLING_SPEED, ADC_0);
    _adc.setSamplingSpeed(ADC_SAMPLING_SPEED, ADC_1);

    _adc.setAveraging(ADC_AVERAGING, ADC_0);
    _adc.setAveraging(ADC_AVERAGING, ADC_1);

    // start ADC
    _adc.startSynchronizedContinuous(_pin0, _pin1);
}

void Engine::setSamplingInterval(int usec) {
    _timer.end();

#ifdef SIMULATE
    waveform0.reset(usec);
    waveform1.reset(usec);
#else
#endif

    _timer.begin(&sampleISR, usec);
}

void Engine::updateTick() {
#ifdef SIMULATE
    int16_t sample0 = waveform0.process();
    int16_t sample1 = waveform1.process();
#else // SIMULATE
    // TODO rescale defined range like 0.0005 volts per increment
    auto result = _adc.readSynchronizedContinuous();
    int16_t sample0 = uint16_t(result.result_adc0) - 0x7fff;
    int16_t sample1 = uint16_t(result.result_adc1) - 0x7fff;
#endif // SIMULATE

    _channels[0].write(sample0);
    _channels[1].write(sample1);
}

void Engine::enableInterrupts() {
    _adc.enableInterrupts(ADC_0);
    _adc.enableInterrupts(ADC_1);
}

void Engine::disableInterrupts() {
    _adc.disableInterrupts(ADC_0);
    _adc.disableInterrupts(ADC_1);
}
