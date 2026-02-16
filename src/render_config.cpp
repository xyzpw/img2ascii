#include <algorithm>
#include "render_config.hpp"
#include "args.hpp"
#include "image.hpp"
#include "utils.hpp"
#include "common.hpp"

ImageConfig makeImageConfig(const Args& args)
{
    ImageConfig config;

    config.scale = args.getValueAsFloat(Command::Scale, 1.0f);
    config.autoscale = args.hasCommand(Command::Autoscale);
    config.asciiChars = args.getValue(Command::Charset, asciiCharset);
    config.charsetDensity = args.getValueAsInt(Command::CharsetDensity, 0);
    config.minBrightness = args.getValueAsFloat(Command::MinBrightness, 0.0f);
    config.trimBackground = args.hasCommand(Command::TrimBackground);
    config.colored = args.hasCommand(Command::Colored);
    config.chroma = args.getValue(Command::Chroma, "");
    config.chromaThreshold = args.getValueAsFloat(Command::ChromaThreshold, 0.5f);
    config.gifTime = args.getValueAsFloat(Command::GifTime, 0.0f);
    config.loopGif = args.hasCommand(Command::LoopGif);
    config.gifCycles = args.getValueAsInt(Command::GifCycles, -1);
    config.gifSpeed = args.getValueAsFloat(Command::GifSpeed, 0.0f);
    config.aspectRatio = args.getValueAsFloat(Command::AspectRatio, 0.5f);
    config.antialias = args.hasCommand(Command::AntiAlias);
    config.saveTextFilename = args.getValue(Command::Save, "");

    if (config.scale <= 0.0f) {
        config.scale = 1.0f;
    }

    config.aspectRatio = std::max(0.01f, config.aspectRatio);

    if (args.hasCommand(Command::CharsetDensity)) {
        int density = config.charsetDensity;
        density = std::clamp(density, 0, (int)charsetDensities.size() - 1);

        config.asciiChars = charsetDensities[density];
    }

    if (!config.chroma.empty()) {
        if (argColorNames.contains(config.chroma)) {
            config.chroma = argColorNames.at(config.chroma);
        }

        auto split = Utils::splitStringToInt(config.chroma, ',');

        if (split.size() != 3) {
            Utils::exitWithError("invalid value for chroma color");
        }

        config.chromaRed = split[0];
        config.chromaGreen = split[1];
        config.chromaBlue = split[2];
    }

    return config;
}
