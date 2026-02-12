#pragma once

#include <string>
#include <map>

enum class Command {
    AntiAlias,
    AspectRatio,
    Charset,
    CharsetDensity,
    Chroma,
    ChromaThreshold,
    Colored,
    File,
    GifCycles,
    GifSpeed,
    GifTime,
    Help,
    LoopGif,
    MinBrightness,
    Scale,
    TrimBackground,
};

struct CommandInfo {
    std::string name;
    std::string usage;
    std::string description;
    bool expectsValue;
};

const std::string projectName = "img2ascii";

inline const std::map<Command, CommandInfo> commandRegistry {
    {
        Command::Charset,
        CommandInfo {
            .name = "charset",
            .usage = "<string>",
            .description = "characters to select from (darkest to brightest)",
            .expectsValue = true,
        },
    },
    {
        Command::MinBrightness,
        CommandInfo {
            .name = "min-brightness",
            .usage = "<decimal>",
            .description = "pixels below this brightness will not be displayed",
            .expectsValue = true,
        },
    },
    {
        Command::TrimBackground,
        CommandInfo {
            .name = "trim",
            .usage = "",
            .description = "trim leading whitespaces and surrounding blank lines from output",
            .expectsValue = false,
        },
    },
    {
        Command::Scale,
        CommandInfo {
            .name = "scale",
            .usage = "<decimal>",
            .description = "scale ascii art size",
            .expectsValue = true,
        },
    },
    {
        Command::Chroma,
        CommandInfo {
            .name = "chroma",
            .usage = "<r,g,b|color>",
            .description = "remove background color from image",
            .expectsValue = true,
        },
    },
    {
        Command::ChromaThreshold,
        CommandInfo {
            .name = "chroma-threshold",
            .usage = "<decimal>",
            .description = "fraction of match at which the color is removed (default 0.5)",
            .expectsValue = true,
        },
    },
    {
        Command::Help,
        CommandInfo {
            .name = "help",
            .usage = "",
            .description = "",
            .expectsValue = false,
        },
    },
    {
        Command::Colored,
        CommandInfo {
            .name = "colored",
            .usage = "",
            .description = "apply pixel colors to ascii characters",
            .expectsValue = false,
        },
    },
    {
        Command::GifTime,
        CommandInfo {
            .name = "gif-time",
            .usage = "<seconds>",
            .description = "duration to play GIF animation (replays if necessary)",
            .expectsValue = true,
        },
    },
    {
        Command::LoopGif,
        CommandInfo {
            .name = "gif-loop",
            .usage = "",
            .description = "loop GIF indefinitely [ctrl+C will terminate]",
            .expectsValue = false,
        },
    },
    {
        Command::GifCycles,
        CommandInfo {
            .name = "gif-cycles",
            .usage = "<number>",
            .description = "number of times to replay a GIF",
            .expectsValue = true,
        },
    },
    {
        Command::GifSpeed,
        CommandInfo {
            .name = "gif-speed",
            .usage = "<decimal>",
            .description = "set fraction of original speed",
            .expectsValue = true,
        },
    },
    {
        Command::CharsetDensity,
        CommandInfo {
            .name = "density",
            .usage = "(0|1|2)",
            .description = "density of ascii characters (range 0-2 as int)",
            .expectsValue = true,
        },
    },
    {
        Command::AspectRatio,
        CommandInfo {
            .name = "aspect",
            .usage = "<decimal>",
            .description = "character height to width ratio for aspect correction (default 0.5)",
            .expectsValue = true,
        },
    },
    {
        Command::AntiAlias,
        CommandInfo {
            .name = "antialias",
            .usage = "",
            .description = "area-average sampling",
            .expectsValue = false,
        },
    },
};

void showHelp();

struct Args {
    Args(int argc, char* argv[]);

    std::map<Command, std::string> commands;

    bool hasArgs() { return commands.size() > 0; }
    bool hasFile() { return commands.contains(Command::File); }
    bool isHelp = false;

    void parse(int argc, char* argv[]);
    bool hasCommand(Command cmd) const;
    std::string getValue(Command cmd) const;
    std::string getValue(Command cmd, std::string fallback) const;
    int getValueAsInt(Command cmd) const;
    int getValueAsInt(Command cmd, int fallback) const;
    float getValueAsFloat(Command cmd) const;
    float getValueAsFloat(Command cmd, float fallback) const;
};
