#pragma once

#include "View.h"
#include "WaveformBuffer.h"

class ViewScope : public View {
public:
    static constexpr int16_t TimeBaseWidth = 18;
    static constexpr int16_t TimeBaseCount = 6;

    ViewScope(App &app);
    virtual ~ViewScope();

    virtual void drawStatic() override;
    virtual bool handleKey(Key key) override;

protected:
    virtual void drawHeader() override;
    virtual void drawFooter() override;

    void drawFullScale();
    void drawFullGrid();
    void drawFullGridPart(int16_t x, int16_t y0, int16_t y1);
    void drawStackedScale();
    void drawStackedGrid();
    void drawStackedGridPart(int16_t x, int16_t y0, int16_t y1);

private:
    void timeBaseChanged();
    void rangeChanged();
};
