#pragma once

#include "window.h"

#include <array>

namespace tst::application {

struct glfw_window_hint {
    int target{};
    int value{};
};

using glfw_context_hints = std::array<glfw_window_hint, 31>;

glfw_context_hints glfw_context() noexcept;

} // namespace tst::application
