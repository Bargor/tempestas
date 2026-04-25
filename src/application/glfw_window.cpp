#include "glfw_window.h"

#include <utility>

namespace tst::application {

namespace {
    GLFWmonitor* to_glfw_monitor(const window::fullscreen_mode mode, const device::monitor& monitor) noexcept {
        if (mode == window::fullscreen_mode::fullscreen) {
            return monitor.get_handle();
        }

        return nullptr;
    }
} // namespace

glfw_window::glfw_window(std::string name,
                         core::extent<int32_t> size,
                         const device::monitor& monitor,
                         visibility is_visible,
                         focus has_focus,
                         cursor_mode cursor,
                         fullscreen_mode fullscreen,
                         state window_state) noexcept :
    window(std::move(name), size, is_visible, has_focus, cursor, fullscreen, window_state),
    m_monitor(monitor) {
    const auto& window_size = get_size();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_VISIBLE, to_glfw_visibility(get_visibility()));
    glfwWindowHint(GLFW_FOCUSED, to_glfw_focus(get_focus()));
    glfwWindowHint(GLFW_MAXIMIZED, to_glfw_maximized(get_state()));

    m_glfw_window = glfwCreateWindow(window_size.width,
                                     window_size.height,
                                     get_name().c_str(),
                                     to_glfw_monitor(get_fullscreen_mode(), m_monitor),
                                     nullptr);
    if (m_glfw_window != nullptr) {
        glfwSetInputMode(m_glfw_window, GLFW_CURSOR, to_glfw_cursor_mode(get_cursor_mode()));
        if (get_state() == state::iconified) {
            glfwIconifyWindow(m_glfw_window);
        }
    } else {
        set_visibility(visibility::hidden);
        set_focus(focus::unfocused);
    }
}

glfw_window::~glfw_window() {
    if (m_glfw_window != nullptr) {
        glfwDestroyWindow(m_glfw_window);
        m_glfw_window = nullptr;
    }
}

GLFWwindow* glfw_window::get_handle() const noexcept {
    return m_glfw_window;
}

const device::monitor& glfw_window::get_monitor() const noexcept {
    return m_monitor;
}

int glfw_window::to_glfw_visibility(const visibility mode) noexcept {
    switch (mode) {
        case visibility::visible:
            return GLFW_TRUE;
        case visibility::hidden:
            return GLFW_FALSE;
        default:
            return GLFW_TRUE;
    }
}

int glfw_window::to_glfw_focus(const focus mode) noexcept {
    switch (mode) {
        case focus::focused:
            return GLFW_TRUE;
        case focus::unfocused:
            return GLFW_FALSE;
        default:
            return GLFW_TRUE;
    }
}

int glfw_window::to_glfw_maximized(const state window_state) noexcept {
    if (window_state == state::maximized) {
        return GLFW_TRUE;
    }
    return GLFW_FALSE;
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
