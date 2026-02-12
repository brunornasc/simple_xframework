//
// Created by bruno on 11/02/2026.
//

#ifndef SIMPLE_XFRAMEWORK_COMPONENT_H
#define SIMPLE_XFRAMEWORK_COMPONENT_H
#include "Object.h"

namespace System {
    class Form;

    class Component : public Object {
    public:
        Form *parent;
        GC gc;
        XFontStruct *fontInfo;
        std::string text;
        System::Location *location;
        System::Size *size;
        bool pressed;
        bool hover;

        bool colorsAllocated;
        char *color;
        unsigned long foregroundColor;

        virtual ~Component() = default;
        virtual void draw() = 0;
        virtual void create() = 0;
        virtual int getLeft() = 0;
        virtual int getTop() = 0;
        virtual int getWidth() = 0;
        virtual int getHeight() = 0;

        virtual void _onMouseEnter() {}
        virtual void _onMouseLeave() {}
        virtual void _onMouseMove(MouseEventArgs *event) {}
        virtual void _onMousePress(MouseEventArgs *event) {}
    };
}
#endif //SIMPLE_XFRAMEWORK_COMPONENT_H