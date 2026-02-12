//
// Created by bruno on 11/02/2026.
//
#pragma once

#ifndef SIMPLE_XFRAMEWORK_KDETHEME_H
#define SIMPLE_XFRAMEWORK_KDETHEME_H

#include "Theme.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace System {
    class KDETheme : public Theme {
    private:
        struct KDEConfig {
            std::string colorScheme;
            unsigned long backgroundColor;
            unsigned long foregroundColor;
            unsigned long buttonBackgroundColor;
            unsigned long buttonHoverColor;
            unsigned long buttonTextColor;
            std::string fontFamily;
            int fontSize;
            int buttonWidth;
            int buttonHeight;
        };

        KDEConfig config;

       void loadConfig() {
            // Tenta ler do ~/.config/kdeglobals primeiro
            std::string home = std::getenv("HOME");
            std::string configPath = home + "/.config/kdeglobals";

            std::ifstream file(configPath);
            if (!file.is_open()) {
                // Fallback: /etc/kdeglobals
                file.open("/etc/kdeglobals");
            }

            // Valores padrão para tema escuro (Breeze Dark)
            config.backgroundColor = 0x202224;  // #202224 - fundo escuro
            config.foregroundColor = 0xfcfcfc;   // #fcfcfc - texto branco
            config.buttonBackgroundColor = 0x292c30; // #292c30 - botão escuro
            config.buttonHoverColor = 0x3daee9;   // Azul Breeze
            config.buttonTextColor = 0xfcfcfc;    // #fcfcfc - texto branco
            config.fontFamily = "Noto Sans";
            config.fontSize = 10;
            config.buttonWidth = 80;
            config.buttonHeight = 30;

            if (file.is_open()) {
                std::string line;
                std::string currentSection;

                while (std::getline(file, line)) {
                    if (line.empty() || line[0] == '#') continue;

                    // Detecta seção
                    if (line[0] == '[') {
                        currentSection = line;
                        continue;
                    }

                    // ============ CORES DOS BOTÕES ============
                    if (currentSection.find("[Colors:Button]") != std::string::npos) {
                        if (line.find("BackgroundNormal=") == 0) {
                            std::string color = line.substr(18);
                            config.buttonBackgroundColor = parseColor(color);
                            std::cout << "Button Background: " << color << " -> " << std::hex << config.buttonBackgroundColor << std::dec << std::endl;
                        }
                        else if (line.find("ForegroundNormal=") == 0) {
                            std::string color = line.substr(18);
                            config.buttonTextColor = parseColor(color);
                            std::cout << "Button Text: " << color << " -> " << std::hex << config.buttonTextColor << std::dec << std::endl;
                        }
                    }

                    // ============ CORES DA JANELA ============
                    else if (currentSection.find("[Colors:Window]") != std::string::npos) {
                        if (line.find("BackgroundNormal=") == 0) {
                            std::string color = line.substr(18);
                            config.backgroundColor = parseColor(color);
                            std::cout << "Window Background: " << color << " -> " << std::hex << config.backgroundColor << std::dec << std::endl;
                        }
                        else if (line.find("ForegroundNormal=") == 0) {
                            std::string color = line.substr(18);
                            config.foregroundColor = parseColor(color);
                            std::cout << "Window Foreground: " << color << " -> " << std::hex << config.foregroundColor << std::dec << std::endl;
                        }
                    }

                    // ============ CORES DE SELEÇÃO/HOVER ============
                    else if (currentSection.find("[Colors:Selection]") != std::string::npos) {
                        if (line.find("BackgroundNormal=") == 0) {
                            std::string color = line.substr(18);
                            config.buttonHoverColor = parseColor(color);
                            std::cout << "Selection/Hover Color: " << color << " -> " << std::hex << config.buttonHoverColor << std::dec << std::endl;
                        }
                    }

                    // ============ FONTE ============
                    else if (currentSection.find("[General]") != std::string::npos) {
                        if (line.find("font=") == 0) {
                            config.fontFamily = line.substr(5);
                            std::cout << "Font Family: " << config.fontFamily << std::endl;
                        } else if (line.find("fontSize=") == 0) {
                            config.fontSize = std::stoi(line.substr(9));
                            std::cout << "Font Size: " << config.fontSize << std::endl;
                        }
                    }
                }
                file.close();
            }

            // Debug: mostra configuração final
            std::cout << "=== KDE Theme Config ===" << std::endl;
            std::cout << "BG: " << std::hex << config.backgroundColor << std::dec << std::endl;
            std::cout << "FG: " << std::hex << config.foregroundColor << std::dec << std::endl;
            std::cout << "Button BG: " << std::hex << config.buttonBackgroundColor << std::dec << std::endl;
            std::cout << "Button Text: " << std::hex << config.buttonTextColor << std::dec << std::endl;
            std::cout << "Hover: " << std::hex << config.buttonHoverColor << std::dec << std::endl;
            std::cout << "Font: " << config.fontFamily << " " << config.fontSize << std::endl;
            std::cout << "========================" << std::endl;
        }

        unsigned long parseColor(const std::string& colorStr) {
            // KDE usa formato "R,G,B"
            int r = 0, g = 0, b = 0;
            if (sscanf(colorStr.c_str(), "%d,%d,%d", &r, &g, &b) == 3) {
                return (r << 16) | (g << 8) | b;
            }
            return 0xeff0f1; // fallback
        }

    public:
        KDETheme() {
            loadConfig();
        }

        unsigned long getWindowBackgroundColor(Display* display, int screen) override {
            return config.backgroundColor;
        }

        unsigned long getButtonBackgroundColor(Display* display, int screen) override {
            return config.buttonBackgroundColor;
        }

        unsigned long getButtonForegroundColor(Display* display, int screen) override {
            return config.buttonTextColor;
        }

        unsigned long getLabelForegroundColor(Display* display, int screen) override {
            return config.foregroundColor;
        }

        unsigned long getBorderColor(Display* display, int screen) override {
            return 0xb0b0b0; // Cinza médio
        }

        int getButtonWidth() override { return config.buttonWidth; }
        int getButtonHeight() override { return config.buttonHeight; }
        int getBorderWidth() override { return 2; }
        int getCornerRadius() override { return 3; }

        std::string getFontName() override { return config.fontFamily; }
        int getFontSize() override { return config.fontSize; }

        void drawButton(Display* display, Window window, GC gc,
                XFontStruct* fontInfo,  // ← RECEBE A FONTE JÁ CARREGADA!
                int x, int y, int width, int height,
                const std::string& text, bool pressed) override {

            // Desenha fundo
            XSetForeground(display, gc, pressed ? 0x3daee9 : config.buttonBackgroundColor);
            XFillRectangle(display, window, gc, x, y, width, height);

            // Desenha borda (estilo KDE Breeze)
            XSetForeground(display, gc, getBorderColor(display, DefaultScreen(display)));
            XDrawRectangle(display, window, gc, x, y, width - 1, height - 1);

            // Só desenha texto se tiver fonte válida!
            if (fontInfo && !text.empty()) {
                // Calcula largura do texto usando a fonte cacheada
                int textWidth = XTextWidth(fontInfo, text.c_str(), text.length());

                // Centraliza
                int textX = x + (width - textWidth) / 2;
                int textY = y + (height + fontInfo->ascent - fontInfo->descent) / 2;

                XSetForeground(display, gc, config.buttonTextColor);
                XSetFont(display, gc, fontInfo->fid);  // Garante que a fonte está setada
                XDrawString(display, window, gc, textX, textY, text.c_str(), text.length());
            }
        }

        void drawLabel(Display* display, Window window, GC gc,
                      int x, int y, const std::string& text) override {
            XSetForeground(display, gc, config.foregroundColor);
            XDrawString(display, window, gc, x, y, text.c_str(), text.length());
        }

        void drawWindowBorder(Display* display, Window window, GC gc,
                            int width, int height) override {
            XSetForeground(display, gc, getBorderColor(display, DefaultScreen(display)));
            for (int i = 0; i < getBorderWidth(); i++) {
                XDrawRectangle(display, window, gc, i, i, width - 2*i - 1, height - 2*i - 1);
            }
        }
    };
}
#endif //SIMPLE_XFRAMEWORK_KDETHEME_H