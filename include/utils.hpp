#pragma once

#include <string>
#include <vector>
#include <utility>

namespace Utils
{
    float divide(float numerator, float denominator);
    std::string unquotedText(std::string text);
    void unquote(std::string& text);
    int minLeadingSpacesPerLine(const std::string& text);
    void unindent(std::string& text);
    void trimNewlines(std::string& text);
    std::vector<std::string> splitString(const std::string& text, char delim);
    std::vector<int> splitStringToInt(const std::string& text, char delim);
    bool isInt(std::string text);
    int toInt(std::string text, int fallback = 0);
    float toFloat(std::string text, float fallback = 0);
    bool fileExists(std::string path);
    bool wget(std::string url, std::string output);
    void printError(std::string text);
    void exitWithError(std::string text);
    [[noreturn]] void exit(int status);
    float steadyTime();
    void sleepForMs(int ms);
    std::string promptInput(std::string text);
    std::pair<int, int> getConsoleSize();
}
