#pragma once

#include "ViewLevel.h"
#include "MovingStats.h"
#include "Voltage.h"
#include "TextWidget.h"

class ViewLevelMeter : public ViewLevel {
public:
    ViewLevelMeter(App &app);
    ~ViewLevelMeter();

    virtual void drawStatic() override;
    // virtual void drawDynamic() override;

    virtual void process(const ProcessContext &ctx) override;

private:
    struct Info {
        voltage_t min = 0;
        voltage_t max = 0;
        voltage_t pp = 0;
        voltage_t avg = 0;
    };

    MovingStats _stats[2];
    Info _infos[2];

    TextWidget _widget;
};
