#pragma once

#include <filesystem>

namespace tst::application {

struct program_params {
    std::filesystem::path execution_directory;
};

program_params parse_program_arguments(int argc, char* argv[]);

} // namespace tst::application
