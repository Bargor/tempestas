#pragma once

#include "event.h"
#include "event_processor.h"

struct GLFWwindow;

namespace tst::application {

class glfw_window;

class input_processor {
public:
    input_processor(glfw_window& window, event_processor<event>& events) noexcept;
    ~input_processor();

    input_processor(const input_processor&) = delete;
    input_processor& operator=(const input_processor&) = delete;
    input_processor(input_processor&&) = delete;
    input_processor& operator=(input_processor&&) = delete;

    void process_events();

private:
    static input_processor& from_window(GLFWwindow* handle) noexcept;

    static void on_focus(GLFWwindow* handle, int focused) noexcept;
    static void on_cursor_position(GLFWwindow* handle, double x, double y) noexcept;
    static void on_mouse_button(GLFWwindow* handle, int button, int action, int mods) noexcept;
    static void on_scroll(GLFWwindow* handle, double x_offset, double y_offset) noexcept;
    static void on_key(GLFWwindow* handle, int key, int scancode, int action, int mods) noexcept;
    static void on_iconify(GLFWwindow* handle, int iconified) noexcept;
    static void on_close(GLFWwindow* handle) noexcept;
    static void on_framebuffer_size(GLFWwindow* handle, int width, int height) noexcept;

    template<typename EventSubtype>
    void queue_event(const EventSubtype& payload) noexcept;

    glfw_window& m_window;
    event_processor<event>& m_events_processor;
    event::mouse_position m_last_cursor_position{};
};

} // namespace tst::application
