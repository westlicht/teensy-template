#pragma once

#include "ViewScope.h"

class ViewScopeSingle : public ViewScope {
public:
    ViewScopeSingle(App &app, int channel);
    ~ViewScopeSingle();

    virtual void drawStatic() override;
    virtual void drawDynamic() override;

    virtual void process(const ProcessContext &ctx) override;

private:
    int _channel;
    int _end;

    WaveformBuffer _waveformBuffer;
};
