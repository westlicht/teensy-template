#pragma once

#include "ViewScope.h"

class ViewScopeStacked : public ViewScope {
public:
    ViewScopeStacked(App &app);
    ~ViewScopeStacked();

    virtual void drawStatic() override;
    virtual void drawDynamic() override;

    virtual void process(const ProcessContext &ctx) override;

private:
    int _end;

    WaveformBuffer _waveformBuffer[2];
};
