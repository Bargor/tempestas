#include "arg_parser.h"

#include <string_view>

namespace tst::application {

program_params parse_program_arguments(const int argc, char* argv[]) {
    auto params = program_params{
        .execution_directory = std::filesystem::current_path(),
    };

    for (int argument_index = 1; argument_index < argc; ++argument_index) {
        const std::string_view argument{argv[argument_index]};
        if (argument == "-f" || argument == "--fullscreen") {
            params.fullscreen_mode = window::fullscreen_mode::fullscreen;
        } else if (argument == "-w" || argument == "--windowed") {
            params.fullscreen_mode = window::fullscreen_mode::windowed;
        }
    }

    return params;
}

} // namespace tst::application
