//
// Created by bruno on 11/02/2026.
//

#ifndef SIMPLE_XFRAMEWORK_THEME_H
#define SIMPLE_XFRAMEWORK_THEME_H
#pragma once

#include <string>
#include <X11/Xlib.h>

namespace System {
    // Forward declarations
    class Form;
    class Button;
    class Label;

    class Theme {
    public:
        virtual ~Theme() = default;

        // Cores do tema
        virtual unsigned long getWindowBackgroundColor(Display* display, int screen) = 0;
        virtual unsigned long getButtonBackgroundColor(Display* display, int screen) = 0;
        virtual unsigned long getButtonForegroundColor(Display* display, int screen) = 0;
        virtual unsigned long getLabelForegroundColor(Display* display, int screen) = 0;
        virtual unsigned long getBorderColor(Display* display, int screen) = 0;

        // Dimensões
        virtual int getButtonWidth() = 0;
        virtual int getButtonHeight() = 0;
        virtual int getBorderWidth() = 0;
        virtual int getCornerRadius() = 0;

        // Fontes
        virtual std::string getFontName() = 0;
        virtual int getFontSize() = 0;

        // Desenho específico
        virtual void drawButton(Display* display, Window window, GC gc,
                                   XFontStruct* fontInfo,  // ← ADICIONADO!
                                   int x, int y, int width, int height,
                                   const std::string& text, bool pressed) = 0;

        virtual void drawLabel(Display* display, Window window, GC gc,
                              int x, int y, const std::string& text) = 0;

        virtual void drawWindowBorder(Display* display, Window window, GC gc,
                                     int width, int height) = 0;
    };
}
#endif //SIMPLE_XFRAMEWORK_THEME_H