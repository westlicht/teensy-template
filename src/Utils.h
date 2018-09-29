#pragma once

#include "Common.h"

class Utils {
public:
    static void voltLabel(char *buf, size_t len, int volt) {
        // snprintf(buf, len, "%c%d", volt > 0 ? '+' : ' ', volt);
        snprintf(buf, len, "%+d", volt);
    }
};
