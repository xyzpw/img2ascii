#include <iostream>
#include <string>
#include <csignal>
#include <unistd.h>
#include "draw.hpp"
#include "image.hpp"
#include "gif.hpp"
#include "utils.hpp"
#include "ansi.hpp"

using std::string;
using Utils::divide;

void stopGif(int signum);

// Pointer to gif currently being played.
Gif* activeGif = nullptr;

void drawImage(const Image& img)
{
    string ascii = "";

    float scaleX = gImageConfig.scale;
    float scaleY = gImageConfig.scale * gImageConfig.aspectRatio;

    /* fit image to console screen if it is too large */
    if (gImageConfig.autoscale) {
        int col = 80, rows = 24;
        if (isatty(STDOUT_FILENO)) {
            auto [c, r] = Utils::getConsoleSize();
            col = c;
            rows = r;
        }

        float maxScaleX = divide(col, img.pxWidth);
        float maxScaleY = divide(rows, img.pxHeight);

        scaleX = std::min(maxScaleX, maxScaleY);
        scaleY = scaleX * gImageConfig.aspectRatio;
    }

    int imgWidth = static_cast<int>(img.pxWidth * scaleX);
    int imgHeight = static_cast<int>(img.pxHeight * scaleY);

    for (int row = 0; row < imgHeight; ++row) {
        for (int col = 0; col < imgWidth; ++col) {

            int pxCol = static_cast<int>(divide(col, scaleX));
            int pxRow = static_cast<int>(divide(row, scaleY));

            if (pxCol >= img.pxWidth) pxCol = img.pxWidth - 1;
            if (pxRow >= img.pxHeight) pxRow = img.pxHeight - 1;

            const Image::Pixel& px = img.getPixel(pxCol, pxRow);

            char c = gImageConfig.antialias
                     ? img.nearbyAverageToAscii(pxCol, pxRow)
                     : img.toAscii(px);

            if (!gImageConfig.colored) {
                ascii += c;
            }
            else {
                ascii += (c == ' ')
                    ? " "
                    : Ansi::colorize(string(1, c), px.r, px.g, px.b);
            }
        }

        ascii.push_back('\n');
    }

    if (gImageConfig.trimBackground) {
        Utils::trimNewlines(ascii);
        Utils::unindent(ascii);
    }

    std::cout << ascii << std::flush;
}

/*
 * Plays the specified GIF; takes into account config.
*/
void drawGif(Gif& gif)
{
    const float& gifTime = gImageConfig.gifTime;
    const int& gifCycles = gImageConfig.gifCycles;
    const bool& loop = gImageConfig.loopGif;
    const int frames = gif.frames.size();

    activeGif = &gif;
    gif.playing = true;
    int cycles = 0;
    float timeAtStart = Utils::steadyTime();

    // seconds since the GIF started playing
    auto elapsed = [&]() {
        return Utils::steadyTime() - timeAtStart;
    };

    // check if the GIF has been playing longer than the custom given duration by user
    auto timeExpired = [&]() {
        return gifTime > 0 && elapsed() > gifTime;
    };

    // return specified time adjusted to custom gif speed
    auto fixedDelay = [&](int ms) {
        if (gImageConfig.gifSpeed > 0)
            return static_cast<int>(divide(ms, gImageConfig.gifSpeed));
        return ms;
    };

    const bool canReplay = (gifTime > 0) || (gifCycles > 0) || loop;

    int frameIndex = 0;

    signal(SIGINT, stopGif);

    Ansi::setCursorVisible(false);
    Ansi::clearScreen();

    while (gif.playing) {
        if (timeExpired())
            break;

        const Frame& frame = gif.frames[frameIndex];

        Ansi::clearScreen();
        drawImage(frame.image);

        Utils::sleepForMs(fixedDelay(frame.delayMs));

        ++frameIndex;

        if (frameIndex == frames) {
            ++cycles;

            if (!canReplay)
                break;

            if (gifCycles > 0 && cycles >= gifCycles)
                break;

            frameIndex = 0;
        }
    }

    stopGif(SIGTERM);
}

void stopGif(int signum)
{
    if (activeGif != nullptr) {
        activeGif->playing = false;
        activeGif = nullptr;
    }

    signal(SIGINT, SIG_DFL);

    Ansi::setCursorVisible(true);
}
