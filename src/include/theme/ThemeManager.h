//
// Created by bruno on 11/02/2026.
//

#ifndef SIMPLE_XFRAMEWORK_THEMEMANAGER_H
#define SIMPLE_XFRAMEWORK_THEMEMANAGER_H
#pragma once

#include "Theme.h"
#include "KDETheme.h"
// #include "GnomeTheme.h"  // Para implementar depois
// #include "WindowsTheme.h" // Para implementar depois
#include <memory>
#include <string>

namespace System {
    // APENAS DECLARAÇÃO (extern)
    extern std::string CurrentDesktopEnvironment;

    class ThemeManager {
    private:
        static std::unique_ptr<Theme> currentTheme;
        static std::string currentDesktop;

    public:
        static void initialize();
        static Theme* getCurrentTheme();
        static void detectAndLoadTheme();
        static void setTheme(std::unique_ptr<Theme> theme);
    };
}
#endif //SIMPLE_XFRAMEWORK_THEMEMANAGER_H