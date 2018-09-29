#pragma once

#include "View.h"

class ViewLevel : public View {
public:
    ViewLevel(App &app);
    virtual ~ViewLevel();

    virtual void drawStatic() override;
    virtual bool handleKey(Key key) override;

protected:
    virtual void drawHeader() override;
    virtual void drawFooter() override;
};
