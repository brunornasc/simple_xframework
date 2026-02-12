#pragma once

#include "Component.h"
#include <string>

#include "theme/ThemeManager.h"

namespace System {
    class Theme;

    class Button : public Component {
    private:
        Form* parent;
        GC gc;
        XFontStruct* fontInfo;
        std::string text;
        System::Location* location;
        System::Size* size;
        bool pressed;
        bool hover;

    public:
        template<typename T>
        Button(const T* form) : pressed(false), hover(false) {
            this->parent = static_cast<Form*>(const_cast<T*>(form));
            this->location = new System::Location();
            this->size = new System::Size();

            // Aplica tema
            Theme* theme = ThemeManager::getCurrentTheme();
            this->size->width = theme->getButtonWidth();
            this->size->height = theme->getButtonHeight();
            this->location->left = 0;
            this->location->top = 0;
        }

        ~Button() override {
            if (fontInfo) {
                XFreeFont(parent->getDisplay(), fontInfo);
            }
            XFreeGC(parent->getDisplay(), gc);
            delete location;
            delete size;
        }

        void setText(const std::string& text) {
            this->text = text;
        }

        void setLocation(int x, int y) {
            this->location->left = x;
            this->location->top = y;
        }

        void setSize(int width, int height) {
            this->size->width = width;
            this->size->height = height;
        }

        void create() override {
            this->gc = XCreateGC(parent->getDisplay(), parent->getWindow(), 0, nullptr);

            Theme* theme = ThemeManager::getCurrentTheme();
            std::string fontName = theme->getFontName() + "-" + std::to_string(theme->getFontSize());

            this->fontInfo = XLoadQueryFont(parent->getDisplay(), fontName.c_str());
            if (!this->fontInfo) {
                this->fontInfo = XLoadQueryFont(parent->getDisplay(), "fixed");
            }

            if (this->fontInfo) {
                XSetFont(parent->getDisplay(), this->gc, this->fontInfo->fid);
            }
        }

        void draw() override {
            Theme* theme = ThemeManager::getCurrentTheme();
            theme->drawButton(
                parent->getDisplay(),
                parent->getWindow(),
                gc,
                this->fontInfo,
                location->left,
                location->top,
                size->width,
                size->height,
                text,
                pressed
            );
        }

        void setPressed(bool p) {
            if (pressed != p) {
                pressed = p;
                // Força redesenho
                XClearArea(parent->getDisplay(), parent->getWindow(),
                          location->left, location->top,
                          size->width, size->height, True);
            }
        }

        bool isPressed() const { return pressed; }
    };
}
