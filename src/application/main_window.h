#pragma once

#include "glfw_window.h"

namespace tst::device {
class monitor;
}

namespace tst::application {

class main_window final : public glfw_window {
public:
    explicit main_window(const device::monitor& monitor) noexcept;
};

} // namespace tst::application
