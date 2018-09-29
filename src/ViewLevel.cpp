#include "ViewLevel.h"

ViewLevel::ViewLevel(App &app) :
    View(app)
{
}

ViewLevel::~ViewLevel() {
}

void ViewLevel::drawStatic() {
    View::drawStatic();
}

//! Key handler.
//! - handles submode selection with up/down keys
bool ViewLevel::handleKey(Key key) {
    if (View::handleKey(key)) {
        return true;
    }
    if (_state.navItem == State::NavItem::SubMode && handleUpDown(_state.level.mode, key)) {
        return true;
    }
    // if (_state.navItem == State::NavItem::Param1 && handleUpDown(_state.scope.timeBase, key)) {
    //     return true;
    // }
    // if (_state.navItem == State::NavItem::Param3 && handleUpDown(_state.scope.range, key)) {
    //     return true;
    // }
    return false;
}

void ViewLevel::drawHeader() {
    View::drawHeader();
    drawNavItem(SubModePos, Texts::Level::Mode[int(_state.level.mode)], _state.navItem == State::NavItem::SubMode);
}

void ViewLevel::drawFooter() {
    View::drawFooter();
    // drawNavItem(ParamPos[0], Texts::Scope::TimeBase[int(_state.scope.timeBase)], _state.navItem == State::NavItem::Param1);
    // drawNavItem(ParamPos[2], Texts::Scope::Range[int(_state.scope.range)], _state.navItem == State::NavItem::Param3);

}
