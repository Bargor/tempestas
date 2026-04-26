#include "glfw_context.h"

#include <GLFW/glfw3.h>

namespace tst::application {

namespace {
    int to_glfw_visibility(const window::visibility_mode mode) noexcept {
        switch (mode) {
            case window::visibility_mode::visible:
                return GLFW_TRUE;
            case window::visibility_mode::hidden:
                return GLFW_FALSE;
            default:
                return GLFW_TRUE;
        }
    }

    int to_glfw_focus(const window::focus_mode mode) noexcept {
        switch (mode) {
            case window::focus_mode::focused:
                return GLFW_TRUE;
            case window::focus_mode::unfocused:
                return GLFW_FALSE;
            default:
                return GLFW_TRUE;
        }
    }

    int to_glfw_maximized(const window::state_mode mode) noexcept {
        if (mode == window::state_mode::maximized) {
            return GLFW_TRUE;
        }
        return GLFW_FALSE;
    }
} // namespace

glfw_context_hints glfw_context(const window::visibility_mode visibility,
                                const window::focus_mode focus,
                                const window::state_mode state) noexcept {
    return {{
        {.target = GLFW_CLIENT_API, .value = GLFW_NO_API},
        {.target = GLFW_VISIBLE, .value = to_glfw_visibility(visibility)},
        {.target = GLFW_FOCUSED, .value = to_glfw_focus(focus)},
        {.target = GLFW_MAXIMIZED, .value = to_glfw_maximized(state)},
    }};
}

} // namespace tst::application
