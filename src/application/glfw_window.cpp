#include "glfw_window.h"

#include <cstdlib>
#include <device/monitor.h>
#include <fmt/printf.h>
#include <utility>

namespace tst::application {

glfw_window::glfw_window(std::string name,
                         core::extent<int32_t> size,
                         const device::monitor* monitor,
                         visibility_mode is_visible,
                         focus_mode has_focus,
                         cursor_mode cursor,
                         fullscreen_mode fullscreen,
                         window_display_state window_state,
                         const glfw_context_hints& context_hints) noexcept
    : window(std::move(name), size, is_visible, has_focus, cursor, fullscreen, window_state), m_monitor(monitor) {
    const auto& window_size = get_size();

    for (const auto& hint : context_hints) {
        glfwWindowHint(hint.target, hint.value);
    }

    // GLFW expects a monitor handle only when creating a fullscreen window.
    const auto get_glfw_monitor = [this]() noexcept -> GLFWmonitor* {
        if (get_fullscreen_mode() == window::fullscreen_mode::fullscreen) {
            if (m_monitor == nullptr) {
                fmt::printf("Failed to create GLFW window: fullscreen mode requires a monitor.\n");
                std::abort();
            }
            return m_monitor->get_handle();
        }
        return nullptr;
    };

    m_glfw_window =
        glfwCreateWindow(window_size.width, window_size.height, get_name().c_str(), get_glfw_monitor(), nullptr);
    if (m_glfw_window == nullptr) {
        const char* glfw_error_description = nullptr;
        glfwGetError(&glfw_error_description);
        fmt::printf("Failed to create GLFW window: %s\n",
                    glfw_error_description != nullptr ? glfw_error_description : "unknown GLFW error");
        std::abort();
    }

    glfwSetInputMode(m_glfw_window, GLFW_CURSOR, to_glfw_cursor_mode(get_cursor_mode()));
    if (get_state() == window_display_state::iconified) {
        glfwIconifyWindow(m_glfw_window);
    }
}

glfw_window::~glfw_window() {
    glfwDestroyWindow(m_glfw_window);
    m_glfw_window = nullptr;
}

GLFWwindow* glfw_window::get_handle() const noexcept {
    return m_glfw_window;
}

const device::monitor* glfw_window::get_monitor() const noexcept {
    return m_monitor;
}

int glfw_window::to_glfw_cursor_mode(const cursor_mode mode) noexcept {
    switch (mode) {
    case cursor_mode::normal:
        return GLFW_CURSOR_NORMAL;
    case cursor_mode::hidden:
        return GLFW_CURSOR_HIDDEN;
    case cursor_mode::disabled:
        return GLFW_CURSOR_DISABLED;
    default:
        return GLFW_CURSOR_NORMAL;
    }
}

} // namespace tst::application
