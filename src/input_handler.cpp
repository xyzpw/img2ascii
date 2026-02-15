#include "input_handler.hpp"
#include "common.hpp"
#include "utils.hpp"
#include <string>
#include <regex>
#include <iostream>

using std::string;

namespace InputHandler
{
    string filenameFromUrl(string url)
    {
        std::regex re(R"(https?://[a-z.]+/.+/(.+?\.[a-z]{2,4})$)");
        std::smatch match;

        if (std::regex_match(url, match, re)) {
            return match[1].str();
        }

        return "";
    }

    bool isUrl(string text)
    {
        if (text.starts_with("http:") || text.starts_with("https:")) {
            return true;
        }
        return false;
    }

    bool isImage(string name)
    {
        for (const string& s : imageExtensions) {
            if (name.ends_with(string("." + s)))
                return true;
        }

        return false;
    }

    void validateFileName(string& filename)
    {
        if (isUrl(filename)) {
            string name = filenameFromUrl(filename);

            if (!isImage(name)) {
                Utils::exitWithError("URL is not valid file type");
            }

            bool downloaded = Utils::wget(filename, name);

            if (!downloaded) {
                Utils::exitWithError("aborted");
            }

            filename = name;
        }

        if (!isImage(filename)) {
            Utils::exitWithError("file type is not an image");
        }
    }
}
