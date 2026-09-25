#include "glfw_window.h"

#include <cassert>
#include <cstdlib>
#include <device/monitor.h>
#include <fmt/printf.h>
#include <utility>

namespace tst::application {

glfw_window::glfw_window(std::string name,
                         core::extent<int32_t> size,
                         const device::monitor* monitor,
                         visibility_mode is_visible,
                         focus_mode has_focus,
                         cursor_mode cursor,
                         fullscreen_mode fullscreen,
                         window_display_state window_state,
                         const glfw_context_hints& context_hints,
                         event_processor<event>& events) noexcept
    : window(std::move(name), size, is_visible, has_focus, cursor, fullscreen, window_state)
    , m_monitor(monitor)
    , m_events_processor(events) {
    const auto& window_size = get_size();

    for (const auto& hint : context_hints) {
        glfwWindowHint(hint.target, hint.value);
    }

    // GLFW expects a monitor handle only when creating a fullscreen window.
    const auto get_glfw_monitor = [this]() noexcept -> GLFWmonitor* {
        if (get_fullscreen_mode() == window::fullscreen_mode::fullscreen) {
            if (m_monitor == nullptr) {
                fmt::printf("Failed to create GLFW window: fullscreen mode requires a monitor.\n");
                std::abort();
            }
            return m_monitor->get_handle();
        }
        return nullptr;
    };

    m_glfw_window =
        glfwCreateWindow(window_size.width, window_size.height, get_name().c_str(), get_glfw_monitor(), nullptr);
    if (m_glfw_window == nullptr) {
        const char* glfw_error_description = nullptr;
        glfwGetError(&glfw_error_description);
        fmt::printf("Failed to create GLFW window: %s\n",
                    glfw_error_description != nullptr ? glfw_error_description : "unknown GLFW error");
        std::abort();
    }

    glfwSetInputMode(m_glfw_window, GLFW_CURSOR, to_glfw_cursor_mode(get_cursor_mode()));
    if (get_state() == window_display_state::iconified) {
        glfwIconifyWindow(m_glfw_window);
    }

    subscribe_to_events();
}

glfw_window::~glfw_window() {
    unsubscribe_from_events();
    glfwDestroyWindow(m_glfw_window);
    m_glfw_window = nullptr;
}

GLFWwindow* glfw_window::get_handle() const noexcept {
    return m_glfw_window;
}

const device::monitor* glfw_window::get_monitor() const noexcept {
    return m_monitor;
}

void glfw_window::close() noexcept {
    assert(glfwWindowShouldClose(m_glfw_window) == GLFW_FALSE);
    glfwSetWindowShouldClose(m_glfw_window, GLFW_TRUE);
    publish_event(event::closed{});
}

void glfw_window::set_size(core::extent<int32_t> size) noexcept {
    assert(get_size() != size);
    window::set_size(size);
    glfwSetWindowSize(m_glfw_window, size.width, size.height);
    publish_event(event::framebuffer_size{size});
}

void glfw_window::set_visibility(visibility_mode is_visible) noexcept {
    assert(get_visibility() != is_visible);
    window::set_visibility(is_visible);
    if (is_visible == visibility_mode::visible) {
        glfwShowWindow(m_glfw_window);
    } else {
        glfwHideWindow(m_glfw_window);
    }
    publish_event(event::visibility{is_visible});
}

void glfw_window::set_focus(focus_mode has_focus) noexcept {
    assert(get_focus() != has_focus);
    window::set_focus(has_focus);
    if (has_focus == focus_mode::focused) glfwFocusWindow(m_glfw_window);
    publish_event(event::focus{has_focus});
}

void glfw_window::set_cursor_mode(cursor_mode mode) noexcept {
    assert(get_cursor_mode() != mode);
    window::set_cursor_mode(mode);
    glfwSetInputMode(m_glfw_window, GLFW_CURSOR, to_glfw_cursor_mode(mode));
    publish_event(event::cursor_mode{mode});
}

void glfw_window::set_state(window_display_state window_state) noexcept {
    assert(get_state() != window_state);
    window::set_state(window_state);
    switch (window_state) {
    case window_display_state::opened:
        glfwRestoreWindow(m_glfw_window);
        break;
    case window_display_state::iconified:
        glfwIconifyWindow(m_glfw_window);
        break;
    case window_display_state::maximized:
        glfwMaximizeWindow(m_glfw_window);
        break;
    default:
        assert(false);
        break;
    }
    publish_event(event::iconify{window_state});
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

template<typename EventSubtype>
void glfw_window::publish_event(const EventSubtype& payload) noexcept {
    [[maybe_unused]] const bool queued = m_events_processor.create_event(payload, this);
    assert(queued);
}

void glfw_window::on_framebuffer_size(core::extent<int32_t> size) noexcept {
    assert(get_size() != size);
    window::set_size(size);
    publish_event(event::framebuffer_size{size});
}

void glfw_window::on_focus(focus_mode mode) noexcept {
    assert(get_focus() != mode);
    window::set_focus(mode);
    publish_event(event::focus{mode});
}

void glfw_window::on_iconify(window_display_state state) noexcept {
    assert(get_state() != state);
    window::set_state(state);
    publish_event(event::iconify{state});
}

void glfw_window::on_close() noexcept {
    assert(glfwWindowShouldClose(m_glfw_window) == GLFW_TRUE);
    publish_event(event::closed{});
}

void glfw_window::subscribe_to_events() noexcept {
    const auto update_frequency = event_processor<event>::duration::zero();
    m_event_subscriptions[0] = m_events_processor.subscribe<event::framebuffer_size>(
        this,
        [this](const event::framebuffer_size& value) {
            assert(get_size() != value.size);
            window::set_size(value.size);
        },
        update_frequency);
    m_event_subscriptions[1] = m_events_processor.subscribe<event::focus>(
        this,
        [this](const event::focus& value) {
            assert(get_focus() != value.mode);
            window::set_focus(value.mode);
        },
        update_frequency);
    m_event_subscriptions[2] = m_events_processor.subscribe<event::visibility>(
        this,
        [this](const event::visibility& value) {
            assert(get_visibility() != value.mode);
            window::set_visibility(value.mode);
        },
        update_frequency);
    m_event_subscriptions[3] = m_events_processor.subscribe<event::iconify>(
        this,
        [this](const event::iconify& value) {
            assert(get_state() != value.state);
            window::set_state(value.state);
        },
        update_frequency);
    m_event_subscriptions[4] = m_events_processor.subscribe<event::cursor_mode>(
        this,
        [this](const event::cursor_mode& value) {
            assert(get_cursor_mode() != value.mode);
            window::set_cursor_mode(value.mode);
        },
        update_frequency);
    m_event_subscriptions[5] = m_events_processor.subscribe<event::closed>(
        this,
        [this](const event::closed&) {
            assert(glfwWindowShouldClose(m_glfw_window) == GLFW_FALSE);
            glfwSetWindowShouldClose(m_glfw_window, GLFW_TRUE);
        },
        update_frequency);
}

void glfw_window::unsubscribe_from_events() noexcept {
    for (const auto subscription : m_event_subscriptions) {
        [[maybe_unused]] const bool unsubscribed = m_events_processor.unsubscribe(subscription);
        assert(unsubscribed);
    }
}

} // namespace tst::application
