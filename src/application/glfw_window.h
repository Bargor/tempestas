#pragma once

#include "glfw_context.h"
#include "window.h"

#include <GLFW/glfw3.h>
#include <device/monitor.h>

namespace tst::application {

class glfw_window final : public window {
public:
    // GLFW does not control vsync when using GLFW_NO_API; this flag describes
    // the desired present behavior for the Vulkan swapchain configuration.
    enum class vsync {
        enabled,
        disabled,
    };

    glfw_window(std::string name,
                core::extent<int32_t> size,
                const device::monitor& monitor,
                visibility_mode is_visible,
                focus_mode has_focus,
                cursor_mode cursor,
                fullscreen_mode fullscreen,
                state_mode window_state,
                vsync vsync_mode,
                const glfw_context_hints& context_hints) noexcept;
    ~glfw_window() override;

    glfw_window(const glfw_window&) = delete;
    glfw_window& operator=(const glfw_window&) = delete;
    glfw_window(glfw_window&&) = delete;
    glfw_window& operator=(glfw_window&&) = delete;

    GLFWwindow* get_handle() const noexcept;
    const device::monitor& get_monitor() const noexcept;

    void set_vsync(vsync vsync_mode) noexcept;
    vsync get_vsync() const noexcept;

private:
    static int to_glfw_cursor_mode(cursor_mode mode) noexcept;

private:
    GLFWwindow* m_glfw_window{};
    const device::monitor& m_monitor;
    vsync m_vsync{vsync::enabled};
};

} // namespace tst::application
