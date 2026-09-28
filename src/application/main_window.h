#pragma once

#include "glfw_window.h"

#include <string>

namespace tst::device {
class monitor;
}

namespace tst::application {

class main_window final : public glfw_window {
public:
    main_window(std::string name,
                core::extent<int32_t> size,
                const device::monitor& monitor,
                fullscreen_mode fullscreen,
                event_processor<event>& events) noexcept;
};

} // namespace tst::application
