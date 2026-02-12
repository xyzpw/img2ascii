#pragma once

#include <string>

namespace Ansi
{
    constexpr std::string ESC = "\x1b";
    constexpr std::string CLEAR = "[2J";
    constexpr std::string CLEAR_SAVED = "[3J";
    constexpr std::string HOME = "[H";
    constexpr std::string CURSOR_ON = "[?25h";
    constexpr std::string CURSOR_OFF = "[?25l";

    void clearScreen();
    void moveCursorHome();
    void setCursorVisible(bool value);
    std::string colorize(const std::string& text, int r, int g, int b);
}
