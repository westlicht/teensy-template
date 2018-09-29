#pragma once

#include "ViewScope.h"

class ViewScopeLayered : public ViewScope {
public:
    ViewScopeLayered(App &app);
    ~ViewScopeLayered();

    virtual void drawStatic() override;
    virtual void drawDynamic() override;

    virtual void process(const ProcessContext &ctx) override;

private:
    int _end;

    WaveformBuffer _waveformBuffer[2];
};
