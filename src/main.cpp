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

    InputHandler::validateFileName(args.commands[Command::File]);
    std::string filename = args.getValue(Command::File);

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
