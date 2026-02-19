//
// Created by bruno on 2026-02-19.
//
#include <string>
#include "include/Button.h"
#include "include/utils/Utils.h"

namespace System {

        Button::~Button() {
            if (fontInfo) {
                XFreeFont(parent->getDisplay(), fontInfo);
            }
            XFreeGC(parent->getDisplay(), gc);
            delete location;
            delete size;
        }

        void Button::setText(const std::string &text) { this->text = text; }

        void Button::setLocation(int x, int y) {
            location->left = x;
            location->top = y;
        }

        void Button::setSize(int width, int height) {
            size->width = width;
            size->height = height;
        }

        void Button::create() {
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

        void Button::draw() {
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

        void Button::setPressed(bool p) {
            if (pressed != p) {
                pressed = p;
                XClearArea(parent->getDisplay(), parent->getWindow(),
                           location->left, location->top,
                           size->width, size->height, True);
            }
        }

        bool Button::isPressed() const { return pressed; }
        void Button::setHover(bool h) { hover = h; }

        int Button::getLeft()  { return location->left; }
        int Button::getTop()  { return location->top; }
        int Button::getWidth()  { return size->width; }
        int Button::getHeight()  { return size->height; }

        void Button::_onMouseEnter() {
            hovered = true;
            // Força redesenho para mostrar hover
            XClearArea(parent->getDisplay(), parent->getWindow(),
                      location->left, location->top,
                      size->width, size->height, True);
        }

        void Button::_onMouseLeave() {
            hovered = false;
            // Força redesenho para remover hover
            XClearArea(parent->getDisplay(), parent->getWindow(),
                      location->left, location->top,
                      size->width, size->height, True);
        }

        void Button::_onMousePress(MouseEventArgs *event) {
            if (this->onClick != nullptr)
                this->onClick->execute(event);
        }

        void Button::_onMouseMove(MouseEventArgs *event) {
            if (this->onMouseMove != nullptr)
                this->onMouseMove->execute(event);
        }
    }
