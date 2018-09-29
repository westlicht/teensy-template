#include "ViewScopeXY.h"

ViewScopeXY::ViewScopeXY(App &app) :
    ViewScope(app)
{
}

ViewScopeXY::~ViewScopeXY() {
}

void ViewScopeXY::drawStatic() {
    clearContent();
}

void ViewScopeXY::drawDynamic() {
    // clear old points
    
    // draw new points
}

void ViewScopeXY::process(const ProcessContext &ctx) {

}
