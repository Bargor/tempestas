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
                         visibility_mode is_visible,
                         focus_mode has_focus,
                         cursor_mode cursor,
                         fullscreen_mode fullscreen,
                         state_mode window_state,
                         vsync vsync_mode,
                         const glfw_context_hints& context_hints) noexcept :
    window(std::move(name), size, is_visible, has_focus, cursor, fullscreen, window_state),
    m_monitor(monitor),
    m_vsync(vsync_mode) {
    // GLFW only creates the native window surface here (GLFW_NO_API). Actual
    // vsync behavior should be selected later via Vulkan present mode.
    const auto& window_size = get_size();

    for (const auto& hint : context_hints) {
        glfwWindowHint(hint.target, hint.value);
    }

    m_glfw_window = glfwCreateWindow(window_size.width,
                                     window_size.height,
                                     get_name().c_str(),
                                     to_glfw_monitor(get_fullscreen_mode(), m_monitor),
                                     nullptr);
    if (m_glfw_window != nullptr) {
        glfwSetInputMode(m_glfw_window, GLFW_CURSOR, to_glfw_cursor_mode(get_cursor_mode()));
        if (get_state() == state_mode::iconified) {
            glfwIconifyWindow(m_glfw_window);
        }
    } else {
        set_visibility(visibility_mode::hidden);
        set_focus(focus_mode::unfocused);
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

void glfw_window::set_vsync(const vsync vsync_mode) noexcept {
    // This stores intent for the Vulkan renderer; GLFW does not apply vsync
    // when the window is created with GLFW_NO_API.
    m_vsync = vsync_mode;
}

glfw_window::vsync glfw_window::get_vsync() const noexcept {
    return m_vsync;
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
