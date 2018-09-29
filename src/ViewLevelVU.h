#pragma once

#include "ViewLevel.h"

class ViewLevelVU : public ViewLevel {
public:
    ViewLevelVU(App &app) :
        ViewLevel(app)
    {
    }

    ~ViewLevelVU() {
    }

    virtual void drawStatic() override {
        ViewLevel::drawStatic();
    }

protected:

private:

};
