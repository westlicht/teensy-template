#include "View.h"
#include "Text.h"

View::View(App &app) :
    _app(app),
    _painter(app.painter()),
    _state(app.state()),
    _theme(app.theme()),
    _engine(app.engine())
{
}

View::~View() {
}

void View::drawNavigation() {
    drawHeader();
    drawFooter();
}

void View::drawStatic() {
    clearContent();
}

void View::drawDynamic() {
}

void View::process(const ProcessContext &ctx) {
}

//! Key handler.
//! - handles selecting navigation item with left/right keys
//! - handles mode selection with up/down keys
bool View::handleKey(Key key) {
    if (handleLeftRight(_state.navItem, key, 0, int(State::NavItem::Count) - 3 + _paramCount)) {
        return true;
    }
    if (_state.navItem == State::NavItem::Mode && handleUpDown(_state.mode, key)) {
        return true;
    }
    return false;
}

void View::setParamCount(uint8_t paramCount) {
    _paramCount = paramCount;
}

void View::drawHeader() {
    _painter.fillRect(0, 0, ScreenSize.x, TopNavHeight, _theme.background);
    drawNavItem(ModePos, Texts::Mode[int(_state.mode)], _state.navItem == State::NavItem::Mode);
}

void View::drawFooter() {
    _painter.fillRect(0, ScreenSize.y - BottomNavHeight, ScreenSize.x, BottomNavHeight, _theme.background);
}

void View::drawNavItem(const Point &pos, const char *text, bool highlighted) {
    _painter.setFont(Painter::Font::Default);
    _painter.setCursor(pos.x, pos.y);
    if (highlighted) {
        _painter.setTextColor(_theme.navigationTextHighlightForeground, _theme.navigationTextHighlightBackground);
    } else {
        _painter.setTextColor(_theme.navigationText, _theme.background);
    }
    _painter.print(text);
}

void View::clearContent() {
    _painter.fillRect(0, TopNavHeight, ScreenSize.x, ScreenSize.y - TopNavHeight - BottomNavHeight, _theme.background);
}
