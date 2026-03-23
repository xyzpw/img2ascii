#include "input_handler.hpp"
#include "common.hpp"
#include "utils.hpp"
#include "image.hpp"
#include <string>
#include <regex>
#include <iostream>
#include <format>

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

    bool isTxtFile(string name)
    {
        return name.ends_with(".txt");
    }

    /*
     * Handles the ascii-to-image argument.
     *
     * @param argValue The entire value of the arg, e.g. in.txt out.png
    */
    void handleAsciiToImage(const string &argValue)
    {
        auto files = Utils::splitString(argValue, ' ');

        bool saved = createImageFromAscii(files.at(0), files.at(1));
        std::cout << (saved ? "image saved" : "failed to save image") << std::endl;
    }

    void validateFileName(string& filename)
    {
        if (isUrl(filename)) {
            string name = filenameFromUrl(filename);

            if (!isImage(name)) {
                Utils::exitWithError("URL is not valid file type");
            }

	    string promptText = std::format("file name (default {}): ", name);

	    /* prompt for output file name */
	    string newName = Utils::promptInput(promptText);
	    if (isImage(newName))
		    name = newName;
	    else
		    std::cerr << "using default name\n";

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
