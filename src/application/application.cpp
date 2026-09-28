#include "application.h"

#include <device/monitor.h>
#include <utility>

namespace tst::application {

namespace {
    core::extent<int32_t> get_main_window_size(const device::monitor& monitor,
                                               const window::fullscreen_mode fullscreen) noexcept {
        const auto& video_mode = monitor.get_video_mode();
        auto size = monitor.get_work_area().dimensions;
        if (video_mode.has_value()) {
            size = core::extent<int32_t>{video_mode->width, video_mode->height};
        }

        if (fullscreen == window::fullscreen_mode::windowed) {
            size.width /= 2;
            size.height /= 2;
        }

        return size;
    }
} // namespace

application::application(program_params params, const device::monitor& monitor) noexcept
    : m_program_params(std::move(params))
    , m_main_window("Tempestas",
                    get_main_window_size(monitor, m_program_params.fullscreen_mode),
                    monitor,
                    m_program_params.fullscreen_mode,
                    m_events_processor)
    , m_input_processor(m_main_window, m_events_processor) {
}

void application::run() noexcept {
    while (glfwWindowShouldClose(m_main_window.get_handle()) == GLFW_FALSE) {
        m_input_processor.process_events();
        m_events_processor.process_events();
    }
}

} // namespace tst::application
