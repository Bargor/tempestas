#pragma once

#include "window.h"

#include <GLFW/glfw3.h>
#include <device/monitor.h>

namespace tst::application {

class glfw_window final : public window {
public:
    glfw_window(std::string name,
                core::extent<int32_t> size,
                const device::monitor& monitor,
                visibility is_visible = visibility::visible,
                focus has_focus = focus::focused,
                cursor_mode cursor = cursor_mode::normal,
                fullscreen_mode fullscreen = fullscreen_mode::windowed,
                state window_state = state::opened) noexcept;
    ~glfw_window() override;

    glfw_window(const glfw_window&) = delete;
    glfw_window& operator=(const glfw_window&) = delete;
    glfw_window(glfw_window&&) = delete;
    glfw_window& operator=(glfw_window&&) = delete;

    GLFWwindow* get_handle() const noexcept;
    const device::monitor& get_monitor() const noexcept;

private:
    static int to_glfw_visibility(visibility mode) noexcept;
    static int to_glfw_focus(focus mode) noexcept;
    static int to_glfw_maximized(state window_state) noexcept;
    static int to_glfw_cursor_mode(cursor_mode mode) noexcept;

private:
    GLFWwindow* m_glfw_window{};
    const device::monitor& m_monitor;
};

} // namespace tst::application
