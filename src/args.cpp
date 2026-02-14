#include <iostream>
#include <string>
#include <sstream>
#include <format>
#include <iomanip>
#include "args.hpp"
#include "utils.hpp"

using std::string;
using std::to_string;

/* Sets the out value to the command with the given name; returns true if successful. */
bool tryGetCommand(string name, Command& out)
{
    for (const auto& it : commandRegistry) {
        if (it.second.name == name) {
            out = it.first;
            return true;
        }
    }

    return false;
}

void showHelp()
{
    std::stringstream stream;
    stream << std::left;
    stream << "usage: " << projectName << " <file|url> [option...]\n\noptions:\n";

    for (const auto& it : commandRegistry) {
        CommandInfo info = it.second;

        if (it.first == Command::Help)
            continue;

        bool isShort = info.name.size() == 1;
        bool equals = !isShort && info.expectsValue;

        string noDesc = std::format(
            "  {}{}{}{}",
            (isShort ? "-" : "--"),
            info.name,
            (equals ? "=" : ""),
            info.usage
        );

        stream << std::setw(DESC_COL) << noDesc;

        if (noDesc.length() >= DESC_COL) {
            stream << '\n' << string(DESC_COL, ' ');
        }

        stream << info.description << '\n';
    }

    std::cout << stream.str();
}

Args::Args(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        Command cmd;
        string cmdName = "", cmdValue = "";
        bool foundCmd;

        if (arg.rfind("--", 0) == 0) {
            auto eq = arg.find_first_of('=');

            if (eq != string::npos) {
                cmdName = arg.substr(2, eq - 2);
                cmdValue = arg.substr(eq + 1);

                foundCmd = tryGetCommand(cmdName, cmd);
            }
            else {
                cmdName = arg.substr(2);
                foundCmd = tryGetCommand(cmdName, cmd);
            }
        }
        else if (arg.rfind('-', 0) == 0 && arg.size() > 1) {
            cmdName = to_string(arg[1]);
            foundCmd = tryGetCommand(cmdName, cmd);
        }
        else if (!hasFile() && (i == 1 || i + 1 == argc)) {
            commands[Command::File] = Utils::unquotedText(arg);
            continue;
        }

        if (!foundCmd) {
            Utils::exitWithError("unknown option '" + arg + "'");
        }

        const CommandInfo& cmdInfo = commandRegistry.at(cmd);

        /* set next arg as value if there is currently no value for the arg
         * and it expects a value */
        if (cmdInfo.expectsValue && cmdValue.empty() && i + 1 <= argc) {
            cmdValue = string(argv[i + 1]);
            ++i;
        }

        if (cmdInfo.expectsValue) {
            string cmdValueUnquoted = Utils::unquotedText(cmdValue);
            string value;

            switch (cmd) {
                case Command::GifCycles:
                case Command::CharsetDensity:
                    value = to_string(Utils::toInt(cmdValueUnquoted));
                    break;
                case Command::Scale:
                case Command::MinBrightness:
                case Command::GifTime:
                case Command::GifSpeed:
                case Command::ChromaThreshold:
                    value = to_string(Utils::toFloat(cmdValueUnquoted));
                    break;
                default:
                    value = cmdValueUnquoted;
                    break;
            }

            commands[cmd] = value;
        }
        else {
            commands[cmd] = "";
        }
    }
}

bool Args::hasCommand(Command command) const
{
    return commands.contains(command);
}

string Args::getValue(Command cmd) const
{
    return commands.at(cmd);
}

string Args::getValue(Command cmd, string fallback) const
{
    if (hasCommand(cmd))
        return commands.at(cmd);

    return fallback;
}

int Args::getValueAsInt(Command cmd) const
{
    return Utils::toInt(getValue(cmd));
}

int Args::getValueAsInt(Command cmd, int fallback) const
{
    if (hasCommand(cmd)) {
        return Utils::toInt(commands.at(cmd), fallback);
    }

    return fallback;
}

float Args::getValueAsFloat(Command cmd) const
{
    return Utils::toFloat(getValue(cmd));
}

float Args::getValueAsFloat(Command cmd, float fallback) const
{
    if (hasCommand(cmd)) {
        return Utils::toFloat(commands.at(cmd), fallback);
    }

    return fallback;
}
