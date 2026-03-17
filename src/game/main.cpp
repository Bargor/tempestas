#include <cstdlib>
#include <fmt/printf.h>
#include <GLFW/glfw3.h>

bool initialize_glfw() {
    if (!glfwInit()) {
        return false;
    }
    return true;
}

void shutdown_glfw() {
    glfwTerminate();
}

int main([[maybe_unused]]  int argc, [[maybe_unused]] char* argv[]) {
    if (!initialize_glfw()) {
        return EXIT_FAILURE;
    }

    fmt::printf("GLFW initialized successfully\n");

    shutdown_glfw();
    return EXIT_SUCCESS;
}
