
#include "../include/theme/ThemeManager.h"

namespace System {
    // DEFINIÇÕES (aqui é o lugar correto!)
    std::string CurrentDesktopEnvironment;
    std::unique_ptr<Theme> ThemeManager::currentTheme = nullptr;
    std::string ThemeManager::currentDesktop;

    void ThemeManager::initialize() {
        currentDesktop = System::CurrentDesktopEnvironment;
        detectAndLoadTheme();
    }

    Theme* ThemeManager::getCurrentTheme() {
        if (!currentTheme) {
            initialize();
        }
        return currentTheme.get();
    }

    void ThemeManager::detectAndLoadTheme() {
        if (currentDesktop.find("KDE") != std::string::npos ||
            currentDesktop.find("kde") != std::string::npos ||
            currentDesktop.find("plasma") != std::string::npos) {
            currentTheme = std::make_unique<KDETheme>();
            }
        // else if (currentDesktop.find("GNOME") != std::string::npos ||
        //          currentDesktop.find("gnome") != std::string::npos) {
        //     currentTheme = std::make_unique<GnomeTheme>();
        // }
        else {
            // Fallback para KDE (mais comum)
            currentTheme = std::make_unique<KDETheme>();
        }
    }

    void ThemeManager::setTheme(std::unique_ptr<Theme> theme) {
        currentTheme = std::move(theme);
    }
}