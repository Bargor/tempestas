#pragma once

#include "window.h"

#include <array>

namespace tst::application {

struct glfw_window_hint {
    int target{};
    int value{};
};

using glfw_context_hints = std::array<glfw_window_hint, 4>;

glfw_context_hints glfw_context(window::visibility_mode visibility,
                                window::focus_mode focus,
                                window::state_mode state) noexcept;

} // namespace tst::application
