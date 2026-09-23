#include "input_processor.h"

#include "glfw_window.h"

#include <cassert>
#include <cstdlib>
#include <fmt/printf.h>

namespace tst::application {

input_processor::input_processor(glfw_window& window, event_processor<event>& events) noexcept
    : m_window(window), m_events(events) {
    GLFWwindow* handle = m_window.get_handle();
    glfwGetCursorPos(handle, &m_last_cursor_position.x, &m_last_cursor_position.y);
    if (glfwGetWindowUserPointer(handle) != nullptr) {
        fmt::printf("Failed to register GLFW callbacks: window user pointer is already set.\n");
        std::abort();
    }
    glfwGetError(nullptr);
    glfwSetWindowUserPointer(handle, this);

    const auto register_callback = [handle](auto setter, auto callback, const char* name) {
        const auto previous_callback = setter(handle, callback);
        if (previous_callback != nullptr) {
            setter(handle, previous_callback);
            fmt::printf("Failed to register GLFW %s callback: one is already installed.\n", name);
            std::abort();
        }
    };

    register_callback(glfwSetWindowFocusCallback, on_focus, "focus");
    register_callback(glfwSetCursorPosCallback, on_cursor_position, "cursor position");
    register_callback(glfwSetMouseButtonCallback, on_mouse_button, "mouse button");
    register_callback(glfwSetScrollCallback, on_scroll, "scroll");
    register_callback(glfwSetKeyCallback, on_key, "key");
    register_callback(glfwSetWindowIconifyCallback, on_iconify, "iconify");
    register_callback(glfwSetWindowCloseCallback, on_close, "close");
    register_callback(glfwSetFramebufferSizeCallback, on_framebuffer_size, "framebuffer size");

    const char* error_description = nullptr;
    if (glfwGetError(&error_description) != GLFW_NO_ERROR) {
        fmt::printf("Failed to register GLFW callbacks: %s\n",
                    error_description != nullptr ? error_description : "unknown GLFW error");
        std::abort();
    }
}

input_processor::~input_processor() {
    GLFWwindow* handle = m_window.get_handle();
    glfwSetWindowFocusCallback(handle, nullptr);
    glfwSetCursorPosCallback(handle, nullptr);
    glfwSetMouseButtonCallback(handle, nullptr);
    glfwSetScrollCallback(handle, nullptr);
    glfwSetKeyCallback(handle, nullptr);
    glfwSetWindowIconifyCallback(handle, nullptr);
    glfwSetWindowCloseCallback(handle, nullptr);
    glfwSetFramebufferSizeCallback(handle, nullptr);
    glfwSetWindowUserPointer(handle, nullptr);
}

void input_processor::process_events() {
    glfwPollEvents();
    m_events.process_events();
}

input_processor& input_processor::from_window(GLFWwindow* handle) noexcept {
    auto* processor = static_cast<input_processor*>(glfwGetWindowUserPointer(handle));
    assert(processor != nullptr);
    return *processor;
}

void input_processor::on_focus(GLFWwindow* handle, int focused) noexcept {
    from_window(handle).queue_event(event::focus{
        focused == GLFW_TRUE ? window::focus_mode::focused : window::focus_mode::unfocused,
    });
}

void input_processor::on_cursor_position(GLFWwindow* handle, double x, double y) noexcept {
    auto& processor = from_window(handle);
    if (processor.m_window.get_cursor_mode() == window::cursor_mode::disabled) {
        const event::mouse_position relative_position{x - processor.m_last_cursor_position.x,
                                                      y - processor.m_last_cursor_position.y};
        processor.queue_event(relative_position);
    } else {
        processor.queue_event(event::mouse_position{x, y});
    }
    processor.m_last_cursor_position = {x, y};
}

void input_processor::on_mouse_button(GLFWwindow* handle, int button, int action, int mods) noexcept {
    from_window(handle).queue_event(event::mouse_button{
        static_cast<device::mouse_button>(button),
        static_cast<device::mouse_action>(action),
        mods,
    });
}

void input_processor::on_scroll(GLFWwindow* handle, double x_offset, double y_offset) noexcept {
    from_window(handle).queue_event(event::scroll{x_offset, y_offset});
}

void input_processor::on_key(GLFWwindow* handle, int key, int scancode, int action, int mods) noexcept {
    from_window(handle).queue_event(event::keyboard{
        static_cast<device::keys>(key),
        scancode,
        static_cast<device::key_action>(action),
        mods,
    });
}

void input_processor::on_iconify(GLFWwindow* handle, int iconified) noexcept {
    from_window(handle).queue_event(event::iconify{
        iconified == GLFW_TRUE ? window::window_display_state::iconified : window::window_display_state::opened,
    });
}

void input_processor::on_close(GLFWwindow* handle) noexcept {
    from_window(handle).queue_event(event::closed{});
}

void input_processor::on_framebuffer_size(GLFWwindow* handle, int width, int height) noexcept {
    from_window(handle).queue_event(event::framebuffer_size{{width, height}});
}

void input_processor::queue_event(event::payload payload) noexcept {
    [[maybe_unused]] const bool queued = m_events.create_event(event{&m_window, payload});
    assert(queued);
}

} // namespace tst::application
