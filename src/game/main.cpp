#include "application/application.h"
#include "application/arg_parser.h"
#include "device/monitor.h"
#include "rendering/instance.h"
#include "rendering/utils.h"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fmt/printf.h>

namespace tst {

bool initialize_glfw() {
    if (!glfwInit()) {
        return false;
    }
    return true;
}

void shutdown_glfw() {
    glfwTerminate();
}

} // namespace tst

int main(int argc, char* argv[]) {
    const auto program_params = tst::application::parse_program_arguments(argc, argv);

    if (!tst::initialize_glfw()) {
        return EXIT_FAILURE;
    }

    fmt::printf("GLFW initialized successfully\n");

    tst::device::monitor primary_monitor(glfwGetPrimaryMonitor());
    tst::application::application application(program_params, primary_monitor);

    [[maybe_unused]] const auto& vulkan_instance = tst::rendering::instance::get_instance();
    fmt::printf("Vulkan instance created successfully\n");

    application.run();

    tst::shutdown_glfw();
    return EXIT_SUCCESS;
}
