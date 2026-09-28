#include "main_window.h"

#include "glfw_context.h"

#include <utility>

namespace tst::application {

main_window::main_window(std::string name,
                         const core::extent<int32_t> size,
                         const device::monitor& monitor,
                         const fullscreen_mode fullscreen,
                         event_processor<event>& events) noexcept
    : glfw_window(std::move(name),
                  size,
                  &monitor,
                  window::visibility_mode::visible,
                  window::focus_mode::focused,
                  window::cursor_mode::normal,
                  fullscreen,
                  window::window_display_state::opened,
                  glfw_context(),
                  events) {
}

} // namespace tst::application
