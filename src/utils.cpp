#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <cassert>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include "utils.hpp"

using std::string;
using std::stringstream;

namespace Utils
{
    float divide(float numerator, float denominator)
    {
        assert(denominator != 0.0f);
        return (numerator / denominator);
    }

    string unquotedText(string text)
    {
        char first = text.at(0);
        char last = text.at(text.length() - 1);

        bool firstIsQuote = first == '\'' || first == '"';

        if (firstIsQuote && last == first) {
            text = text.substr(1, text.length() - 2);
        }

        return text;
    }

    /*
     * Remove single or double quotes from the beginning and end of text;
     * does nothing if string is not wrapped in matching quotes.
    */
    void unquote(string& text)
    {
        char first = text.at(0);
        char last = text.at(text.length() - 1);

        bool firstIsQuote = first == '\'' || first == '"';

        if (firstIsQuote && last == first) {
            text = text.substr(1, text.length() - 2);
        }
    }

    /*
     * Return minimum number of leading spaces out of each line in the string.
    */
    int minLeadingSpacesPerLine(const string& text)
    {
        std::istringstream stream(text);
        std::string line;
        int minSpaces = std::numeric_limits<int>::max();

        while (std::getline(stream, line)) {
            if (line.empty()) continue;

            int count = 0;

            for (char c : line) {
                if (c == ' ')
                    ++count;
                else
                    break;
            }

            if (count < minSpaces)
                minSpaces = count;
        }

        return minSpaces;
    }

    /*
     * Finds the line with the fewest leading spaces and removes that many spaces
     * at the start of every line.
    */
    void unindent(string& text)
    {
        stringstream stream(text);
        string line;
        string newValue;

        int minSpaces = minLeadingSpacesPerLine(text);

        while (std::getline(stream, line)) {
            newValue.append(line.substr(minSpaces) + "\n");
        }

        text = newValue;
    }

    /*
     * Remove all empty lines from top and bottom of string.
    */
    void trimNewlines(string& text)
    {
        stringstream stream(text);
        string line;
        string newValue;

        while (std::getline(stream, line)) {
            if (!line.empty() && line.find_first_not_of(' ') != string::npos) {
                newValue.append(line + "\n");
            }
        }

        text = newValue;
    }

    std::vector<string> splitString(const string& text, char delim)
    {
        std::stringstream stream(text);
        string line;

        std::vector<string> result;

        while (std::getline(stream, line, delim)) {
            result.push_back(line);
        }

        return result;
    }

    std::vector<int> splitStringToInt(const string& text, char delim)
    {
        std::vector<string> v = splitString(text, delim);
        std::vector<int> result;

        for (auto& i : v) {
            if (!isInt(i))
                continue;
            result.push_back(std::stoi(i));
        }

        return result;
    }

    bool isInt(string text)
    {
        for (string::iterator it = text.begin(); it != text.end(); ++it) {
            if (!std::isdigit(*it))
                return false;
        }

        return true;
    }

    int toInt(std::string text, int fallback)
    {
        try {
            return std::stoi(text);
        }
        catch (...) {
            return fallback;
        }
    }

    float toFloat(std::string text, float fallback)
    {
        try {
            return std::stof(text);
        }
        catch (...) {
            return fallback;
        }
    }

    bool fileExists(string path)
    {
        return std::filesystem::exists(path);
    }

    /*
     * Runs `wget` shell command (asks for confirmation before running!).
     *
     * @param url URL
     * @param output name of the file to write to
    */
    bool wget(string url, string output)
    {
        string cmd = std::format("wget -O {} '{}'", output, url);

        std::string prompt = std::format("run shell command '{}'? [y/N] ", cmd.c_str());

        string perm = promptInput(prompt);

        if (perm == "Y" || perm == "y") {
            std::system(cmd.c_str());
            return true;
        }

        return false;
    }

    void printError(string text)
    {
        std::cerr << "error: " << text << std::endl;
    }

    void exitWithError(string text)
    {
        printError(text);
        std::exit(EXIT_FAILURE);
    }

    [[noreturn]] void exit(int status)
    {
        std::exit(status);
    }

    float steadyTime()
    {
        auto tp = std::chrono::steady_clock::now();
        std::chrono::duration<double> dur = tp.time_since_epoch();

        return dur.count();
    }

    void sleepForMs(int ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    string promptInput(string text)
    {
        std::cout << text;

        string input;
        std::getline(std::cin, input);

        return input;
    }
}
