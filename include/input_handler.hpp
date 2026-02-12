#pragma once

#include <string>

namespace InputHandler
{
    std::string filenameFromUrl(std::string url);
    bool isUrl(std::string text);
    bool isImage(std::string name);
}
