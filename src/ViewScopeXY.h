#pragma once

#include "ViewScope.h"

class ViewScopeXY : public ViewScope {
public:
    ViewScopeXY(App &app);
    ~ViewScopeXY();

    virtual void drawStatic() override;
    virtual void drawDynamic() override;

    virtual void process(const ProcessContext &ctx) override;



private:
    
};
