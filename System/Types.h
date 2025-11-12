#include "X11/Xlib.h"

#ifndef __TYPES_H__

namespace System {
    typedef XEvent EventArgs;
    typedef XKeyEvent KeyEventArgs;
    typedef XButtonEvent MouseEventArgs;

    struct Location {
        signed int Left;
        signed int Top;
    };

    struct Size {
        unsigned int Width;
        unsigned int Height;
    };
}

#define __TYPES_H__
#endif
