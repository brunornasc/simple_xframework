#pragma once

#include "Types.h"
#include <X11/Xlib.h>
#include <string>

#include "Component.h"
#include "Form.h"

namespace System {
    class Label : public Component {
    private:
        Display *display;
        int screen;
        Window parent;
        GC gc;
        XFontStruct *fontInfo;
        unsigned long foregroundColor;

    public:
        System::Location *location;
        System::Size *size;
        std::string text;
        char *color;

        Label(Display *display, Window parent, int screen);
        template<typename T>
        Label(const T* form) : Label(form->getDisplay(), form->getWindow(), form->getScreen()) {}
        ~Label() override;

        void setText(const std::string &text);
        void setLocation(int x, int y);
        void setSize(int width, int height);
        void draw() override;
    };
}
