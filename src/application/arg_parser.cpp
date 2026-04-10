#include "arg_parser.h"

namespace tst::application {

program_params parse_program_arguments([[maybe_unused]] const int argc, [[maybe_unused]] char* argv[]) {
    return program_params{
        .execution_directory = std::filesystem::current_path(),
    };
}

} // namespace tst::application
