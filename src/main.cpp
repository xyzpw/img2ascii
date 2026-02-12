#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include "input_handler.hpp"
#include "image.hpp"
#include "args.hpp"
#include "render_config.hpp"
#include "utils.hpp"
#include "draw.hpp"
#include "gif.hpp"

int main(int argc, char* argv[])
{
    Args args(argc, argv);
    gImageConfig = makeImageConfig(args);

    if (!args.hasArgs() || args.hasCommand(Command::Help)) {
        showHelp();
        return 0;
    }
    else if (!args.hasFile()) {
        Utils::exitWithError("no file");
    }

    std::string filename = args.commands[Command::File];
    bool isUrl = InputHandler::isUrl(filename);

    /* download URL and fix the arg command value to the downloaded file */
    if (isUrl) {
        std::string name = InputHandler::filenameFromUrl(filename);

        // exit with error if the name is not an image type
        if (!InputHandler::isImage(name)) {
            Utils::exitWithError("URL is not a valid file type");
        }

        /* try to download file; exit with error if failed */
        if (Utils::wget(filename, name)) {
            args.commands[Command::File] = name;
            filename = name;
        }
        else {
            Utils::exitWithError("cancelled");
        }
    }

    /* display with a different method if a GIF is specified */
    if (boost::iends_with(filename, ".gif")) {
        Gif gif = loadGif(filename);
        drawGif(gif);

        return 0;
    }

    Image img(args.getValue(Command::File));

    drawImage(img);

    return 0;
}
