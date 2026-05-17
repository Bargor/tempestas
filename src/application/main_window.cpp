#include "main_window.h"

#include "glfw_context.h"

#include <device/monitor.h>

namespace tst::application {

main_window::main_window(const device::monitor& monitor) noexcept
    : glfw_window("Tempestas",
                  core::extent<int32_t>{1280, 720},
                  &monitor,
                  window::visibility_mode::visible,
                  window::focus_mode::focused,
                  window::cursor_mode::normal,
                  window::fullscreen_mode::windowed,
                  window::window_display_state::opened,
                  glfw_context()) {
}

} // namespace tst::application
