#pragma once
#include "X11/Xlib.h"
#include <string>
#include "Form.h"
#include "Label.h"
#include "Button.h"
#include "theme/ThemeManager.h"

#ifndef SYSTEM_H_
#define SYSTEM_H_

namespace System {
    extern std::string CurrentDesktopEnvironment;

    static void DetectDesktopEnvironment() {
        const char* env_p = std::getenv("XDG_SESSION_DESKTOP");
        if (env_p) {
            CurrentDesktopEnvironment = env_p;
        } else {
            CurrentDesktopEnvironment = "unknown";
        }

        ThemeManager::initialize();
    }
}

#endif