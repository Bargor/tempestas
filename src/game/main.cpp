#include <cstdlib>
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

int main(int argc, char* argv[]) {
    if (!initialize_glfw()) {
        return EXIT_FAILURE;
    }

    shutdown_glfw();
    return EXIT_SUCCESS;
}
