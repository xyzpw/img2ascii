#pragma once

#include <string>
#include <vector>
#include <utility>

const std::string asciiCharset = "@%#*+=-:. ";

struct ImageConfig {
    std::string asciiChars = asciiCharset;
    int charsetDensity = 0;
    float scale = 1.0f;
    float minBrightness = 0.0f;
    float gifTime = 0.0f;
    bool trimBackground = false;
    bool colored = false;
    bool loopGif = false;
    int gifCycles = -1;
    float gifSpeed = 0.0f;
    float aspectRatio = 0.5f;
    bool antialias = false;

    std::string chroma = "";
    float chromaThreshold = 0.0f;
    int chromaRed = 0;
    int chromaGreen = 0;
    int chromaBlue = 0;
};

inline ImageConfig gImageConfig;

struct Image {
    struct Pixel {
        int r, g, b, a;
        int gray;
        int alpha;
        int row;
        float brightness() const;
        float distanceSqrd(int r, int g, int b) const;

        Pixel(){}
        Pixel(int r, int g, int b, int a)
            : r(r), g(g), b(b), a(a) {}
    };

    int pxWidth, pxHeight;
    int pxSize;
    int channel;
    std::vector<Pixel> pixels;

    std::string asciiChars = asciiCharset;

    Image(std::string path);
    Image(int width, int height, std::vector<Pixel>&& px)
        : pxWidth(width), pxHeight(height), channel(4), pixels(std::move(px)) {}

    char toAscii(const Pixel& px) const;
    char toAscii(const std::vector<const Pixel*>& block) const;
    char nearbyAverageToAscii(int col, int row) const;
    std::string toAsciiColored(const Pixel& px) const;
    char brightnessToAscii(float brightness) const;
    const Pixel& getPixel(int col, int row) const;
    int chromaConfigDistanceSqrd(const Pixel& px) const;
    bool isChromaMatch(const Pixel& px) const;
};
