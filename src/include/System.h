#pragma once
#include "X11/Xlib.h"
#include <string>
#include "Form.h"

#ifndef SYSTEM_H_
#define SYSTEM_H_

//__SYSTEM__H
//AQUI TEMOS A COLEÇÃO DE TODAS AS CLASSES DENTRO DO NAMESPACE SYSTEM
//INCLUIR #include<system/object.h>

namespace System {
    extern std::string CurrentDesktopEnvironment;

    static void DetectDesktopEnvironment() {
        const char* env_p = std::getenv("XDG_SESSION_DESKTOP");
        if (env_p) {
            CurrentDesktopEnvironment = env_p;
        } else {
            CurrentDesktopEnvironment = "unknown";
        }
    }
}

#endif