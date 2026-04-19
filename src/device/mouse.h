#pragma once

#include <GLFW/glfw3.h>
#include <cstdint>

namespace tst::device {

enum class mouse_buttons : uint8_t {
    button_left = GLFW_MOUSE_BUTTON_LEFT,
    button_right = GLFW_MOUSE_BUTTON_RIGHT,
    button_middle = GLFW_MOUSE_BUTTON_MIDDLE
};

enum class mouse_action : uint8_t { release = GLFW_RELEASE, press = GLFW_PRESS, repeat = GLFW_REPEAT };

} // namespace tst::device
