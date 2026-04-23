#pragma once

#include <GLFW/glfw3.h>
#include <core/position.h>
#include <core/rectangle.h>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace tst::device {

class monitor {
public:
    struct video_mode {
        int32_t width{};
        int32_t height{};
        int32_t red_bits{};
        int32_t green_bits{};
        int32_t blue_bits{};
        int32_t refresh_rate{};
    };

    struct gamma_ramp {
        std::vector<uint16_t> red;
        std::vector<uint16_t> green;
        std::vector<uint16_t> blue;
    };

public:
    explicit monitor(GLFWmonitor* monitor) noexcept;

    void refresh() noexcept;

    GLFWmonitor* get_handle() const noexcept {
        return m_monitor;
    }

    const std::string& get_name() const noexcept {
        return m_name;
    }

    bool is_primary() const noexcept {
        return m_is_primary;
    }

    const core::position<int32_t>& get_position() const noexcept {
        return m_position;
    }

    const core::extent<int32_t>& get_physical_size_mm() const noexcept {
        return m_physical_size_mm;
    }

    const core::rectangle<int32_t, int32_t>& get_work_area() const noexcept {
        return m_work_area;
    }

    const core::position<float>& get_content_scale() const noexcept {
        return m_content_scale;
    }

    const std::optional<video_mode>& get_video_mode() const noexcept {
        return m_video_mode;
    }

    const gamma_ramp& get_gamma_ramp() const noexcept {
        return m_gamma_ramp;
    }

private:
    GLFWmonitor* m_monitor{};
    std::string m_name;
    bool m_is_primary{false};
    core::position<int32_t> m_position{};
    core::extent<int32_t> m_physical_size_mm{};
    core::rectangle<int32_t, int32_t> m_work_area{};
    core::position<float> m_content_scale{};
    std::optional<video_mode> m_video_mode{};
    gamma_ramp m_gamma_ramp{};
};

} // namespace tst::device
