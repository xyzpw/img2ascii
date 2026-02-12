#pragma once

#include "image.hpp"
#include <vector>
#include <string>

struct Frame {
    Image image;
    int delayMs;
    Frame(Image& img, int delay)
        : image(img), delayMs(delay) {}
};

struct Gif {
    std::vector<Frame> frames;
    bool playing = false;
    Gif() {};
};

Gif loadGif(std::string path);
