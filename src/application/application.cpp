#include "application.h"

#include <utility>

namespace tst::application {

application::application(program_params params, const device::monitor& monitor) noexcept
    : m_program_params(std::move(params))
    , m_main_window(monitor, m_events_processor)
    , m_input_processor(m_main_window, m_events_processor) {
}

void application::run() noexcept {
    while (glfwWindowShouldClose(m_main_window.get_handle()) == GLFW_FALSE) {
        m_input_processor.process_events();
    }
}

} // namespace tst::application
