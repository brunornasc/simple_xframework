#pragma once

#include "Component.h"
#include "Form.h"
#include "theme/ThemeManager.h"
#include <string>

#include "utils/Utils.h"

#ifndef BUTTON_H_
#define BUTTON_H_

namespace System {
    class Button : public Component {
        unsigned long buttonBgColor;
        unsigned long buttonHoverColor;
        unsigned long buttonTextColor;
        bool hovered;

    public:
        System::EventHandler *onClick = nullptr;
        System::EventHandler *onMouseMove = nullptr;

        template<typename T>
        Button(const T *form) {
            this->parent = static_cast<Form *>(const_cast<T *>(form));
            this->pressed = false;
            this->hover = false;
            this->colorsAllocated = false;
            this->location = new System::Location();
            this->size = new System::Size();

            // Pega DADOS do tema, não renderização
            Theme *theme = ThemeManager::getCurrentTheme();
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

        void setText(const std::string &text) { this->text = text; }

        void setLocation(int x, int y) {
            location->left = x;
            location->top = y;
        }

        void setSize(int width, int height) {
            size->width = width;
            size->height = height;
        }

        void create() override {
            Display *dpy = parent->getDisplay();
            Window win = parent->getWindow();
            int screen = parent->getScreen();
            Colormap cmap = DefaultColormap(dpy, screen);

            this->gc = XCreateGC(dpy, win, 0, nullptr);

            Theme *theme = ThemeManager::getCurrentTheme();

            XColor xcolor;

            Utils::parseXColor(&xcolor, theme->getButtonBackgroundColor());

            if (!XAllocColor(dpy, cmap, &xcolor)) {
                std::cerr << "FALHA ao alocar BG! Usando fallback" << std::endl;
                buttonBgColor = WhitePixel(dpy, screen);

            } else {
                buttonBgColor = xcolor.pixel;

            }

            Utils::parseXColor(&xcolor, theme->getButtonTextColor());

            if (!XAllocColor(dpy, cmap, &xcolor)) {
                std::cerr << "FALHA ao alocar Text! Usando fallback" << std::endl;
                buttonTextColor = BlackPixel(dpy, screen);

            } else {
                buttonTextColor = xcolor.pixel;

            }

            // Hover: #3daee9
            Utils::parseXColor(&xcolor, theme->getButtonHoverColor());

            if (!XAllocColor(dpy, cmap, &xcolor)) {
                std::cerr << "FALHA ao alocar Hover! Usando fallback" << std::endl;
                buttonHoverColor = buttonBgColor;
            } else {
                buttonHoverColor = xcolor.pixel;

            }

            // Fonte
            std::string fontName = theme->getFontName();
            this->fontInfo = XLoadQueryFont(dpy, fontName.c_str());
            if (!this->fontInfo) {
                this->fontInfo = XLoadQueryFont(dpy, "fixed");
            }
            if (this->fontInfo) {
                XSetFont(dpy, this->gc, this->fontInfo->fid);
            }
        }

        void draw() override {
            if (!parent || !parent->getDisplay() || !parent->getWindow()) return;

            Display *dpy = parent->getDisplay();
            Window win = parent->getWindow();

            // Desenha fundo - USA A COR ALOCADA!
            XSetForeground(dpy, gc, buttonBgColor);
            XFillRectangle(dpy, win, gc,
                           location->left, location->top,
                           size->width, size->height);

            // Desenha texto - USA A COR ALOCADA!
            if (fontInfo && !text.empty()) {
                int textWidth = XTextWidth(fontInfo, text.c_str(), text.length());
                int textX = location->left + (size->width - textWidth) / 2;
                int textY = location->top + (size->height + fontInfo->ascent - fontInfo->descent) / 2;

                XSetForeground(dpy, gc, buttonTextColor);
                XSetFont(dpy, gc, fontInfo->fid);
                XDrawString(dpy, win, gc,
                            textX, textY,
                            text.c_str(), text.length());
            }
        }

        void setPressed(bool p) {
            if (pressed != p) {
                pressed = p;
                XClearArea(parent->getDisplay(), parent->getWindow(),
                           location->left, location->top,
                           size->width, size->height, True);
            }
        }

        bool isPressed() const { return pressed; }
        void setHover(bool h) { hover = h; }

        int getLeft() override { return location->left; }
        int getTop() override { return location->top; }
        int getWidth() override { return size->width; }
        int getHeight() override { return size->height; }

        void _onMouseEnter() override {
            hovered = true;
            // Força redesenho para mostrar hover
            XClearArea(parent->getDisplay(), parent->getWindow(),
                      location->left, location->top,
                      size->width, size->height, True);
        }

        void _onMouseLeave() override {
            hovered = false;
            // Força redesenho para remover hover
            XClearArea(parent->getDisplay(), parent->getWindow(),
                      location->left, location->top,
                      size->width, size->height, True);
        }

        void _onMousePress(MouseEventArgs *event) override {
            if (this->onClick != nullptr)
                this->onClick->execute(event);
        }

        void _onMouseMove(MouseEventArgs *event) override {
            if (this->onMouseMove != nullptr)
                this->onMouseMove->execute(event);
        }
    };
}
#endif
