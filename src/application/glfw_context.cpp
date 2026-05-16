#include "glfw_context.h"

#include <GLFW/glfw3.h>

namespace tst::application {

glfw_context_hints glfw_context() noexcept {
    return {{
        {.target = GLFW_CLIENT_API, .value = GLFW_NO_API},
        {.target = GLFW_CONTEXT_CREATION_API, .value = GLFW_NATIVE_CONTEXT_API},
        {.target = GLFW_CONTEXT_VERSION_MAJOR, .value = 3},
        {.target = GLFW_CONTEXT_VERSION_MINOR, .value = 5},
        {.target = GLFW_CONTEXT_ROBUSTNESS, .value = GLFW_NO_ROBUSTNESS},
        {.target = GLFW_CONTEXT_RELEASE_BEHAVIOR, .value = GLFW_ANY_RELEASE_BEHAVIOR},
        {.target = GLFW_CONTEXT_NO_ERROR, .value = GLFW_FALSE},

        {.target = GLFW_VISIBLE, .value = GLFW_TRUE},
        {.target = GLFW_FOCUSED, .value = GLFW_TRUE},
        {.target = GLFW_ICONIFIED, .value = GLFW_FALSE},
        {.target = GLFW_RESIZABLE, .value = GLFW_TRUE},
        {.target = GLFW_DECORATED, .value = GLFW_TRUE},
        {.target = GLFW_AUTO_ICONIFY, .value = GLFW_TRUE},
        {.target = GLFW_FLOATING, .value = GLFW_FALSE},
        {.target = GLFW_MAXIMIZED, .value = GLFW_FALSE},
        {.target = GLFW_CENTER_CURSOR, .value = GLFW_TRUE},
        {.target = GLFW_TRANSPARENT_FRAMEBUFFER, .value = GLFW_FALSE},
        {.target = GLFW_FOCUS_ON_SHOW, .value = GLFW_TRUE},
        {.target = GLFW_MOUSE_PASSTHROUGH, .value = GLFW_FALSE},
        {.target = GLFW_SCALE_TO_MONITOR, .value = GLFW_FALSE},
        {.target = GLFW_SCALE_FRAMEBUFFER, .value = GLFW_TRUE},

        {.target = GLFW_RED_BITS, .value = 8},
        {.target = GLFW_GREEN_BITS, .value = 8},
        {.target = GLFW_BLUE_BITS, .value = 8},
        {.target = GLFW_ALPHA_BITS, .value = 8},
        {.target = GLFW_DEPTH_BITS, .value = 24},
        {.target = GLFW_STENCIL_BITS, .value = 8},
        {.target = GLFW_SAMPLES, .value = 1},
        {.target = GLFW_SRGB_CAPABLE, .value = GLFW_FALSE},
        {.target = GLFW_REFRESH_RATE, .value = GLFW_DONT_CARE},
        {.target = GLFW_DOUBLEBUFFER, .value = GLFW_TRUE},
    }};
}

} // namespace tst::application
