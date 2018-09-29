#pragma once

#include "Common.h"

struct Theme {
    uint16_t background = BLACK;

    uint16_t navigationText = WHITE;
    uint16_t navigationTextHighlightForeground = BLACK;
    uint16_t navigationTextHighlightBackground = WHITE; 

    uint16_t scale = WHITE;
    uint16_t scaleTick = WHITE;
    uint16_t scaleLabel = WHITE;

    uint16_t grid = BLUE;

    uint16_t traceSingle = WHITE;
    uint16_t traceLayered0 = RED;
    uint16_t traceLayered1 = GREEN;
    uint16_t traceLayeredBoth = WHITE;

    uint16_t seperator = RED;

};
