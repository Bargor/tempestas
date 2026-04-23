#include "monitor.h"

namespace tst::device {

namespace {
    monitor::video_mode to_video_mode(const GLFWvidmode& glfw_video_mode) noexcept {
        return monitor::video_mode{
            .width = glfw_video_mode.width,
            .height = glfw_video_mode.height,
            .red_bits = glfw_video_mode.redBits,
            .green_bits = glfw_video_mode.greenBits,
            .blue_bits = glfw_video_mode.blueBits,
            .refresh_rate = glfw_video_mode.refreshRate,
        };
    }
} // namespace

monitor::monitor(GLFWmonitor* monitor) noexcept : m_monitor(monitor) {
    refresh();
}

void monitor::refresh() noexcept {
    if (m_monitor == nullptr) {
        m_name.clear();
        m_is_primary = false;
        m_position = {};
        m_physical_size_mm = {};
        m_work_area = {};
        m_content_scale = {};
        m_video_mode.reset();
        m_gamma_ramp = {};
        return;
    }

    const char* monitor_name = glfwGetMonitorName(m_monitor);
    m_name = monitor_name != nullptr ? monitor_name : "";

    m_is_primary = glfwGetPrimaryMonitor() == m_monitor;

    glfwGetMonitorPos(m_monitor, &m_position.x, &m_position.y);
    glfwGetMonitorPhysicalSize(m_monitor, &m_physical_size_mm.width, &m_physical_size_mm.height);
    glfwGetMonitorWorkarea(m_monitor,
                           &m_work_area.offset.x,
                           &m_work_area.offset.y,
                           &m_work_area.dimensions.width,
                           &m_work_area.dimensions.height);
    glfwGetMonitorContentScale(m_monitor, &m_content_scale.x, &m_content_scale.y);

    const GLFWvidmode* current_video_mode = glfwGetVideoMode(m_monitor);
    if (current_video_mode != nullptr) {
        m_video_mode = to_video_mode(*current_video_mode);
    } else {
        m_video_mode.reset();
    }

    const GLFWgammaramp* glfw_gamma_ramp = glfwGetGammaRamp(m_monitor);
    if (glfw_gamma_ramp != nullptr) {
        const auto ramp_size = static_cast<size_t>(glfw_gamma_ramp->size);

        m_gamma_ramp.red.assign(glfw_gamma_ramp->red, glfw_gamma_ramp->red + ramp_size);
        m_gamma_ramp.green.assign(glfw_gamma_ramp->green, glfw_gamma_ramp->green + ramp_size);
        m_gamma_ramp.blue.assign(glfw_gamma_ramp->blue, glfw_gamma_ramp->blue + ramp_size);
    } else {
        m_gamma_ramp = {};
    }
}

} // namespace tst::device
