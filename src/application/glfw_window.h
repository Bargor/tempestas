#pragma once

#include "event.h"
#include "event_processor.h"
#include "glfw_context.h"
#include "window.h"

#include <GLFW/glfw3.h>
#include <array>

namespace tst::device {
class monitor;
}

namespace tst::application {

class glfw_window : public window {
public:
    glfw_window(std::string name,
                core::extent<int32_t> size,
                const device::monitor* monitor,
                visibility_mode is_visible,
                focus_mode has_focus,
                cursor_mode cursor,
                fullscreen_mode fullscreen,
                window_display_state window_state,
                const glfw_context_hints& context_hints,
                event_processor<event>& events) noexcept;
    ~glfw_window() override;

    glfw_window(const glfw_window&) = delete;
    glfw_window& operator=(const glfw_window&) = delete;
    glfw_window(glfw_window&&) = delete;
    glfw_window& operator=(glfw_window&&) = delete;

    GLFWwindow* get_handle() const noexcept;
    const device::monitor* get_monitor() const noexcept;
    void close() noexcept override;
    void set_size(core::extent<int32_t> size) noexcept override;
    void set_visibility(visibility_mode is_visible) noexcept override;
    void set_focus(focus_mode has_focus) noexcept override;
    void set_cursor_mode(cursor_mode mode) noexcept override;
    void set_state(window_display_state window_state) noexcept override;

private:
    static int to_glfw_cursor_mode(cursor_mode mode) noexcept;

    template<typename EventSubtype>
    void publish_event(const EventSubtype& payload) noexcept;

    void on_framebuffer_size(core::extent<int32_t> size) noexcept;
    void on_focus(focus_mode mode) noexcept;
    void on_iconify(window_display_state state) noexcept;
    void on_close() noexcept;

    void subscribe_to_events() noexcept;
    void unsubscribe_from_events() noexcept;

    friend class input_processor;

private:
    GLFWwindow* m_glfw_window{};
    const device::monitor* m_monitor{};
    event_processor<event>& m_events_processor;
    std::array<event_processor<event>::subscription_id, 6> m_event_subscriptions{};
};

} // namespace tst::application
