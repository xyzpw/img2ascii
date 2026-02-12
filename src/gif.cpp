#include <string>
#include <format>
#include <gif_lib.h>
#include "gif.hpp"
#include "utils.hpp"
#include "image.hpp"

using std::string;

Gif loadGif(string path)
{
    if (!Utils::fileExists(path)) {
        string text = std::format("file with name '{}' does not exist", path);
        Utils::exitWithError(text);
    }

    int error = 0;
    GifFileType* gifFile = DGifOpenFileName(path.c_str(), &error);

    if (!gifFile || DGifSlurp(gifFile) != GIF_OK) {
        Utils::exitWithError("failed to read GIF");
    }

    Gif gif;

    for (int i = 0; i < gifFile->ImageCount; ++i) {
        SavedImage& frame = gifFile->SavedImages[i];
        GifImageDesc& desc = frame.ImageDesc;

        int w = desc.Width;
        int h = desc.Height;

        ColorMapObject* cmap =
            desc.ColorMap ? desc.ColorMap : gifFile->SColorMap;

        if (!cmap) {
            Utils::exitWithError(std::format("no color map found for image {}", i));
        }

        std::vector<Image::Pixel> framePixels;

        int transparentIndex = -1;
        int delayMs = 100;

        for (int j = 0; j < frame.ExtensionBlockCount; ++j) {
            ExtensionBlock& ext = frame.ExtensionBlocks[j];

            if (ext.Function == GRAPHICS_EXT_FUNC_CODE) {
                GraphicsControlBlock gcb;
                DGifExtensionToGCB(ext.ByteCount, ext.Bytes, &gcb);

                if (gcb.TransparentColor != NO_TRANSPARENT_COLOR) {
                    transparentIndex = gcb.TransparentColor;
                }

                delayMs = gcb.DelayTime * 10;
            }
        }

        GifByteType* raster = frame.RasterBits;

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                int idx = raster[y * w + x];
                GifColorType& c = cmap->Colors[idx];

                int a = (idx == transparentIndex) ? 0 : 255;

                int red = static_cast<int>(c.Red);
                int green = static_cast<int>(c.Green);
                int blue = static_cast<int>(c.Blue);

                framePixels.emplace_back(red, green, blue, a);
            }
        }

        Image frameImg = Image(w, h, std::move(framePixels));
        Frame gifFrame(frameImg, delayMs);

        gif.frames.push_back(gifFrame);
    }

    DGifCloseFile(gifFile, &error);

    return gif;
}
