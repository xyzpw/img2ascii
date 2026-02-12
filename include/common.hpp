#pragma once

#include <string>
#include <unordered_map>
#include <vector>

const std::unordered_map<std::string, std::string> argColorNames {
    { "green", "0,255,0" },
    { "red", "255,0,0" },
    { "blue", "0,0,255" },
    { "white", "255,255,255" },
    { "black", "0,0,0" },
};

const std::vector<std::string> charsetDensities {
    {"@%#*+=-:. "},
    {"@#\%xo;:,. "},
    {"$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. "},
};

const std::vector<std::string> imageExtensions {
    {"png"},
    {"jpg"},
    {"jpeg"},
    {"gif"},
};
