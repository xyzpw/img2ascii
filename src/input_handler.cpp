#include "input_handler.hpp"
#include "common.hpp"
#include <string>
#include <regex>

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
}
