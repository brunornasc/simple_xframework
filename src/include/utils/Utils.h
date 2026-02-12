//
// Created by bruno on 11/02/2026.
//

#ifndef SIMPLE_XFRAMEWORK_UTILS_H
#define SIMPLE_XFRAMEWORK_UTILS_H
#include <X11/Xlib.h>

namespace System {
    class Utils {
    public:
        static void parseXColor(XColor *source, unsigned long color) {
            source->red = ((color >> 16) & 0xFF) * 257;
            source->green = ((color >> 8) & 0xFF) * 257;
            source->blue = (color & 0xFF) * 257;
            source->flags = DoRed | DoGreen | DoBlue;
        }
    };
}

#endif //SIMPLE_XFRAMEWORK_UTILS_H