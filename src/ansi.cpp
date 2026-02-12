#include "ansi.hpp"
#include <string>
#include <iostream>
#include <format>

using std::string;

namespace Ansi
{
    void clearScreen()
    {
        std::cout << ESC + CLEAR + ESC + CLEAR_SAVED + ESC + HOME;
    }

    void moveCursorHome()
    {
        std::cout << ESC + HOME;
    }

    void setCursorVisible(bool value)
    {
        string code = value ? CURSOR_ON : CURSOR_OFF;
        std::cout << ESC + code;
    }

    string colorize(const string& text, int r, int g, int b)
    {
        string result = std::format("\x1b[38;2;{};{};{}m{}\x1b[0m", r, g, b, text);
        return result;
    }
}
