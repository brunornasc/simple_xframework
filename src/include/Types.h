#pragma once

#include "X11/Xlib.h"

#ifndef TYPES_H_
#define TYPES_H_

namespace System {
    typedef XEvent EventArgs;
    typedef XKeyEvent KeyEventArgs;
    typedef XButtonEvent MouseEventArgs;

    struct Location {
        signed int left;
        signed int top;
    };

    struct Size {
        unsigned int width;
        unsigned int height;
    };
}
#endif
