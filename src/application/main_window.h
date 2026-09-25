#pragma once

#include "glfw_window.h"

namespace tst::device {
class monitor;
}

namespace tst::application {

class main_window final : public glfw_window {
public:
    main_window(const device::monitor& monitor, event_processor<event>& events) noexcept;
};

} // namespace tst::application
