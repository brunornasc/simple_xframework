#pragma once

#include "Types.h"
#include <X11/Xlib.h>
#include <string>
#include "Component.h"

#ifndef LABEL_H_

namespace System {
    class Form;

    class Label : public Component {
    private:
        Form *parent;
        GC gc;
        XFontStruct *fontInfo;
        unsigned long foregroundColor;

    public:
        System::Location *location;
        System::Size *size;
        std::string text;
        char *color;

        template<typename T>
        Label(const T* form) {
            this->parent = static_cast<Form*>(const_cast<T*>(form));

            this->location = new System::Location();
            this->size = new System::Size();
            this->location->left = 0;
            this->location->top = 0;
            this->size->width = 100;
            this->size->height = 20;
        }

        ~Label() override;

        void setText(const std::string &text);
        void setLocation(int x, int y);
        void setSize(int width, int height);
        void draw() override;
        void create() override;
    };
}

#endif