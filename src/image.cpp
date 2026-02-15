#include <format>
#include <cstddef>
#include "image.hpp"
#include "ansi.hpp"
#include "utils.hpp"
#include "input_handler.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/stb_image.h"
#include "../external/stb_image_write.h"

using std::string;
using Utils::divide;

/* Return normalized brightness. */
float Image::Pixel::brightness() const
{
    return divide(r + g + b, 765);
}

float Image::Pixel::distanceSqrd(int r, int g, int b) const
{
    int dr = r - this->r;
    int dg = g - this->g;
    int db = b - this->b;

    return dr*dr + dg*dg + db*db;
}

Image::Image(std::string path)
{
    if (!Utils::fileExists(path)) {
        string text = std::format("file with name '{}' does not exist", path);
        Utils::exitWithError(text);
    }
    else if (path.ends_with("gif")) {
        return;
    }

    channel = 4;

    unsigned char* data = stbi_load(path.c_str(), &pxWidth, &pxHeight, &pxSize, channel);

    if (!data) {
        string text = std::format("failed to load image with file name '{}'", path);
        Utils::exitWithError(text);
    }

    for (int i = 0; i < pxWidth * pxHeight; ++i) {
        int index = i * channel;

        int r = data[index];
        int g = data[index + 1];
        int b = data[index + 2];
        int a = data[index + 3];

        pixels.emplace_back(r, g, b, a);
    }

    stbi_image_free(data);
}

/*
 * Computes pixel data and returns the ideal ascii character.
*/
char Image::toAscii(const Image::Pixel& px) const
{
    float b = px.brightness();

    if (px.a == 0.0f || b < gImageConfig.minBrightness) {
        return ' ';
    }

    if (!gImageConfig.chroma.empty() && isChromaMatch(px)) {
        return ' ';
    }

    int index = b * (gImageConfig.asciiChars.size() - 1);
    return gImageConfig.asciiChars[index];
}

/*
 * Compute pixel data and return colorized ascii character.
 *
 * @returns colorized ascii character as string
*/
string Image::toAsciiColored(const Image::Pixel& px) const
{
    char c = toAscii(px);

    if (c == ' ')
        return string(1, c);

    return Ansi::colorize(string(1, c), px.r, px.g, px.b);
}

char Image::toAscii(const std::vector<const Pixel*>& block) const
{
    float brightness = 0.0f;
    float alpha = 0.0f;
    bool chromaMatch = true;

    for (const auto& p : block) {
        brightness += p->brightness();
        alpha += p->a;
        if (chromaMatch && !isChromaMatch(*p))
            chromaMatch = false;
    }

    brightness = divide(brightness, block.size());
    alpha = divide(alpha, block.size());

    if (alpha == 0.0f || brightness < gImageConfig.minBrightness || chromaMatch)
        return ' ';

    return brightnessToAscii(brightness);
}

char Image::nearbyAverageToAscii(int col, int row) const
{
    std::vector<const Pixel*> block;

    block.push_back(&getPixel(col, row));

    for (int x = -1; x < 1; ++x) {
        for (int y = -1; y < 1; ++y) {
            int curCol = col + x;
            int curRow = row + y;

            bool isCol = curCol >= 0 && curCol < pxWidth;
            bool isRow = curRow >= 0 && curRow < pxHeight;

            if (isCol && isRow) {
                const auto& px = getPixel(curCol, curRow);

                if (px.a == 0.f)
                    continue;

                block.push_back(&px);
            }
        }
    }

    return toAscii(block);
}

char Image::brightnessToAscii(float b) const
{
    if (b < gImageConfig.minBrightness) {
        return ' ';
    }

    int index = b * (gImageConfig.asciiChars.size() - 1);
    return gImageConfig.asciiChars[index];
}

const Image::Pixel& Image::getPixel(int col, int row) const
{
    return pixels.at(row * pxWidth + col);
}

int Image::chromaConfigDistanceSqrd(const Image::Pixel& px) const
{
    return px.distanceSqrd(gImageConfig.chromaRed, gImageConfig.chromaGreen, gImageConfig.chromaBlue);
}

bool Image::isChromaMatch(const Image::Pixel& px) const
{
    int distSqr = px.distanceSqrd(gImageConfig.chromaRed, gImageConfig.chromaGreen, gImageConfig.chromaBlue);
    float match = 1.0f - divide(distSqr, 255*255);

    return match >= gImageConfig.chromaThreshold;
}

float asciiToBrightness(char c)
{
    auto index = gImageConfig.asciiChars.find(c);

    if (index == string::npos) {
        return 0.0f;
    }

    return divide((float)index, (float)gImageConfig.asciiChars.length());
}

/*
 * Reads text file and saves it as an image named as the specified output.
 *
 * @return success
*/
bool createImageFromAscii(const string &txtFile, const string &output)
{
    if (!Utils::fileExists(txtFile) || !InputHandler::isTxtFile(txtFile)) {
        Utils::exitWithError("txt file does not exist");
    }

    /* prompt for confirmation to continue if the output file already exists */
    if (Utils::fileExists(output)) {
        bool perm = Utils::promptYesOrNo("output file already exists, continue with save? ", false);

        if (!perm)
            return false;
    }

    auto fileLines = Utils::readFileLines(txtFile);

    size_t width = 0;
    size_t height = fileLines.size();
    int channels = 4;

    // find longest line
    for (const auto& line : fileLines) {
        if (line.length() > width) {
            width = line.length();
        }
    }

    std::vector<unsigned char> image(width * height * channels);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            int index = (y * width + x) * channels;

            char c = ' ';
            if (x < fileLines[y].length()) {
                c = fileLines[y][x];
            }

            float brightness = std::clamp(asciiToBrightness(c), 0.0f, 1.0f);
            unsigned char bData = static_cast<unsigned char>(brightness * 255.0f);

            image[index + 0] = bData;
            image[index + 1] = bData;
            image[index + 2] = bData;
            image[index + 3] = (c == ' ') ? 0 : 255;
        }
    }

    string promptMessage = std::format("write to file '{}'? ", output.c_str());
    bool perm = Utils::promptYesOrNo(promptMessage, false);

    // exit if user does not give permission
    if (!perm) {
        return false;
    }

    stbi_write_png(
        output.c_str(),
        width,
        height,
        channels,
        image.data(),
        width * channels
    );

    return true;
}
