#pragma once

#include "glfw_context.h"
#include "window.h"

#include <GLFW/glfw3.h>

namespace tst::device {
class monitor;
}

namespace tst::application {

class glfw_window : public window {
public:
    glfw_window(std::string name,
                core::extent<int32_t> size,
                const device::monitor* monitor,
                visibility_mode is_visible,
                focus_mode has_focus,
                cursor_mode cursor,
                fullscreen_mode fullscreen,
                window_display_state window_state,
                const glfw_context_hints& context_hints) noexcept;
    ~glfw_window() override;

    glfw_window(const glfw_window&) = delete;
    glfw_window& operator=(const glfw_window&) = delete;
    glfw_window(glfw_window&&) = delete;
    glfw_window& operator=(glfw_window&&) = delete;

    GLFWwindow* get_handle() const noexcept;
    const device::monitor* get_monitor() const noexcept;

private:
    static int to_glfw_cursor_mode(cursor_mode mode) noexcept;

private:
    GLFWwindow* m_glfw_window{};
    const device::monitor* m_monitor{};
};

} // namespace tst::application
