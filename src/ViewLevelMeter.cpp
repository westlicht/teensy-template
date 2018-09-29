#include "ViewLevelMeter.h"
#include "Voltage.h"
#include "TextWidget.h"

ViewLevelMeter::ViewLevelMeter(App &app) :
    ViewLevel(app),
    _stats({2, 2}),
    _widget(Point(0,0), Point(0,0), Painter::HAlignment::Center, Painter::VAlignment::Center, Painter::Font::Default, WHITE, BLACK)
{
}

ViewLevelMeter::~ViewLevelMeter() {
}

void ViewLevelMeter::drawStatic() {
    clearContent();

    _painter.drawFastHLine(0, 16, 128, _theme.seperator);
    _painter.drawFastHLine(0, 80, 128, _theme.seperator);
    _painter.drawFastHLine(0, 145, 128, _theme.seperator);

    _painter.setFont(Painter::Font::Large);
    _painter.setTextColor(WHITE);
    // _painter.drawRect(0, 16, 16, 64, WHITE);
    // _painter.drawRect(0, 80, 16, 64, WHITE);
    _painter.printAligned(0, 16, 16, 64, Painter::HAlignment::Center, Painter::VAlignment::Center, "1");
    _painter.printAligned(0, 80, 16, 64, Painter::HAlignment::Center, Painter::VAlignment::Center, "2");

    _painter.setFont(Painter::Font::Default);
    _painter.setTextColor(WHITE);
    _painter.setCursor(32, 16+4);
    _painter.print("Low");
    _painter.setCursor(32, 16+4+12);
    _painter.print("High");
    _painter.setCursor(32, 16+4+12+12);
    _painter.print("P-P");
    _painter.setCursor(32, 16+4+12+12+12);
    _painter.print("Avg.");

    _painter.printAt(64, 16+4, 1);
    _painter.printAt(64, 16+4+12, 1);
    _painter.printAt(64, 16+4+12+12, 1);
    _painter.printAt(64, 16+4+12+12+12, 1);

    _painter.setCursor(32, 80+4);
    _painter.print("Low");
    _painter.setCursor(32, 80+4+12);
    _painter.print("High");
    _painter.setCursor(32, 80+4+12+12);
    _painter.print("P-P");
    _painter.setCursor(32, 80+4+12+12+12);
    _painter.print("Avg.");

    // _painter.setCursor(32, 16+4+12+12+12);
}

void drawDynamic() {
    
}

void ViewLevelMeter::process(const ProcessContext &ctx) {
    int index = ctx.begin;
    while (index != ctx.end) {
        _stats[0].update(_engine.channel(0).read(index));
        _stats[1].update(_engine.channel(1).read(index));
        index = _engine.wrap(index + 1);
    }
}
