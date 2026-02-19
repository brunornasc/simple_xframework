#pragma once

#include "Component.h"
#include "Form.h"
#include "theme/ThemeManager.h"
#include <string>

#ifndef BUTTON_H_
#define BUTTON_H_

namespace System {
    class Button : public Component {
        unsigned long buttonBgColor;
        unsigned long buttonHoverColor;
        unsigned long buttonTextColor;
        bool hovered;

    public:
        EventHandler *onClick = nullptr;
        EventHandler *onMouseMove = nullptr;

        void draw() override;
        void create() override;
        int getLeft() override;
        int getTop() override;
        int getWidth() override;
        int getHeight() override;
        void _onMouseEnter() override;
        void _onMouseLeave() override;
        void _onMouseMove(MouseEventArgs *event) override;
        void _onMousePress(MouseEventArgs *event) override;

        void setText(const std::string &text);
        void setLocation(int x, int y);
        void setSize(int width, int height);
        void setPressed(bool p);
        bool isPressed() const;
        void setHover(bool h);

        template<typename T>
        Button(const T *form) {
            this->parent = static_cast<Form *>(const_cast<T *>(form));
            this->pressed = false;
            this->hover = false;
            this->colorsAllocated = false;
            this->location = new System::Location();
            this->size = new System::Size();

            Theme *theme = ThemeManager::getCurrentTheme();
            this->size->width = theme->getButtonWidth();
            this->size->height = theme->getButtonHeight();
            this->location->left = 0;
            this->location->top = 0;
        }

        ~Button();
    };
}
#endif
