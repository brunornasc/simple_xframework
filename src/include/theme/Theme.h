#pragma once

#include <string>
#include <X11/Xlib.h>
#ifndef THEME_H_
#define THEME_H_
namespace System {
    class Theme {
    public:
        virtual ~Theme() = default;
        
        // Cores - APENAS RETORNA VALORES!
        virtual unsigned long getWindowBackgroundColor() = 0;
        virtual unsigned long getWindowForegroundColor() = 0;
        virtual unsigned long getButtonBackgroundColor() = 0;
        virtual unsigned long getButtonHoverColor() = 0;
        virtual unsigned long getButtonTextColor() = 0;
        virtual unsigned long getLabelTextColor() = 0;
        virtual unsigned long getBorderColor() = 0;
        
        // Dimensões
        virtual int getButtonWidth() = 0;
        virtual int getButtonHeight() = 0;
        virtual int getBorderWidth() = 0;
        virtual int getCornerRadius() = 0;
        
        // Fontes - NOME DA FONTE NO SISTEMA X11!
        virtual std::string getFontName() = 0;
        virtual int getFontSize() = 0;
    };
}
#endif