#pragma once

#include "window.h"

#include <filesystem>

namespace tst::application {

struct program_params {
    std::filesystem::path execution_directory;
    window::fullscreen_mode fullscreen_mode{window::fullscreen_mode::windowed};
};

program_params parse_program_arguments(int argc, char* argv[]);

} // namespace tst::application
