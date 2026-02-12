//
// Created by bruno on 11/02/2026.
//
#pragma once

#ifndef SIMPLE_XFRAMEWORK_KDETHEME_H
#define SIMPLE_XFRAMEWORK_KDETHEME_H
#pragma once

#include "Theme.h"
#include <fstream>
#include <iomanip>

namespace System {
    class KDETheme : public Theme {
    private:
        struct KDEConfig {
            unsigned long windowBg;
            unsigned long windowFg;
            unsigned long buttonBg;
            unsigned long buttonHover;
            unsigned long buttonText;
            unsigned long labelText;
            unsigned long borderColor;
            std::string fontFamily;
            int fontSize;
            int buttonWidth;
            int buttonHeight;
        } config;

        unsigned long parseColor(const std::string& colorStr) {
            int r = 0, g = 0, b = 0;
            if (sscanf(colorStr.c_str(), "%d,%d,%d", &r, &g, &b) == 3) {
                const unsigned long color = (r << 16) | (g << 8) | b;
                return color;
            }
            return 0xeff0f1;
        }

        std::string convertFontName(const std::string& kdeFont) {
            if (kdeFont.find("Noto Sans") != std::string::npos)
                return "-*-noto sans-*-*-*-*-*-*-*-*-*-*-*-*";
            if (kdeFont.find("Liberation Sans") != std::string::npos)
                return "-*-liberation sans-*-*-*-*-*-*-*-*-*-*-*-*";
            if (kdeFont.find("DejaVu Sans") != std::string::npos)
                return "-*-dejavu sans-*-*-*-*-*-*-*-*-*-*-*-*";
            if (kdeFont.find("Arial") != std::string::npos)
                return "-*-arial-*-*-*-*-*-*-*-*-*-*-*-*";
            return "fixed";
        }

    public:
        KDETheme() {
            // Valores padrão (Breeze Dark)
            config.windowBg = 0x202224;
            config.windowFg = 0xfcfcfc;
            config.buttonBg = 0x292c30;
            config.buttonHover = 0x3daee9;
            config.buttonText = 0xfcfcfc;
            config.labelText = 0xfcfcfc;
            config.borderColor = 0x3c3f43;
            config.fontFamily = "-*-noto sans-*-*-*-*-*-*-*-*-*-*-*-*";
            config.fontSize = 10;
            config.buttonWidth = 80;
            config.buttonHeight = 30;

            loadKDEConfig();
        }

        void loadKDEConfig() {
            std::string home = std::getenv("HOME");
            std::string configPath = home + "/.config/kdeglobals";
            std::ifstream file(configPath);

            if (!file.is_open()) return;

            std::string line;
            std::string currentSection;

            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#') continue;

                if (line[0] == '[') {
                    currentSection = line;
                    continue;
                }

                // Cores dos botões
                if (currentSection.find("[Colors:Button]") != std::string::npos) {
                    if (line.find("BackgroundNormal=") == 0)
                        config.buttonBg = parseColor(line.substr(17));
                    else if (line.find("ForegroundNormal=") == 0)
                        config.buttonText = parseColor(line.substr(17));
                }

                // Cores da janela
                else if (currentSection.find("[Colors:Window]") != std::string::npos) {
                    if (line.find("BackgroundNormal=") == 0)
                        config.windowBg = parseColor(line.substr(17));
                    else if (line.find("ForegroundNormal=") == 0) {
                        config.windowFg = parseColor(line.substr(17));
                        config.labelText = config.windowFg;
                    }
                }

                // Hover/Selection
                else if (currentSection.find("[Colors:Selection]") != std::string::npos) {
                    if (line.find("BackgroundNormal=") == 0)
                        config.buttonHover = parseColor(line.substr(17));
                }

                // Fontes
                else if (currentSection.find("[General]") != std::string::npos) {
                    if (line.find("font=") == 0) {
                        config.fontFamily = convertFontName(line.substr(4));
                    } else if (line.find("fontSize=") == 0) {
                        config.fontSize = std::stoi(line.substr(8));
                    }
                }
            }
            file.close();
        }

        unsigned long getWindowBackgroundColor() override { return config.windowBg; }
        unsigned long getWindowForegroundColor() override { return config.windowFg; }
        unsigned long getButtonBackgroundColor() override { return config.buttonBg; }
        unsigned long getButtonHoverColor() override { return config.buttonHover; }
        unsigned long getButtonTextColor() override { return config.buttonText; }
        unsigned long getLabelTextColor() override { return config.labelText; }
        unsigned long getBorderColor() override { return config.borderColor; }

        int getButtonWidth() override { return config.buttonWidth; }
        int getButtonHeight() override { return config.buttonHeight; }
        int getBorderWidth() override { return 2; }
        int getCornerRadius() override { return 3; }

        std::string getFontName() override { return config.fontFamily; }
        int getFontSize() override { return config.fontSize; }
    };
}
#endif //SIMPLE_XFRAMEWORK_KDETHEME_H