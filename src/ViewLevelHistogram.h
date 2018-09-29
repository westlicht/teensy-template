#pragma once

#include "ViewLevel.h"
#include "HistogramBuffer.h"

class ViewLevelHistogram : public ViewLevel {
public:
    ViewLevelHistogram(App &app);
    ~ViewLevelHistogram();

    virtual void drawStatic() override;
    virtual void drawDynamic() override;

    virtual void process(const ProcessContext &ctx) override;

    virtual bool handleKey(Key key) override;

protected:
    virtual void drawFooter() override;

private:
    void rangeChanged();

    HistogramBuffer _histogramBuffer[2];

    uint16_t _colors[HistogramBuffer::IntensityCount];
};
