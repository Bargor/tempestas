#include <cstdlib>
#include <fmt/printf.h>
#include <GLFW/glfw3.h>
#include "rendering/instance.h"
#include "rendering/utils.h"

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

}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    if (!tst::initialize_glfw()) {
        return EXIT_FAILURE;
    }

    fmt::printf("GLFW initialized successfully\n");

    [[maybe_unused]] const auto& vulkanInstance = tst::rendering::instance::get_instance();
    fmt::printf("Vulkan instance created successfully\n");

    tst::shutdown_glfw();
    return EXIT_SUCCESS;
}