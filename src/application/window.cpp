#include "window.h"

#include <utility>

namespace tst::application {

window::window(std::string name,
               core::extent<int32_t> size,
               window::visibility is_visible,
               window::focus has_focus,
               window::cursor_mode cursor,
               window::fullscreen_mode fullscreen,
               window::state window_state) noexcept :
    m_name(std::move(name)),
    m_size(size),
    m_is_visible(is_visible),
    m_has_focus(has_focus),
    m_cursor_mode(cursor),
    m_fullscreen_mode(fullscreen),
    m_window_state(window_state) {
}

void window::set_name(std::string name) noexcept {
    m_name = std::move(name);
}

const std::string& window::get_name() const noexcept {
    return m_name;
}

void window::set_size(core::extent<int32_t> size) noexcept {
    m_size = size;
}

const core::extent<int32_t>& window::get_size() const noexcept {
    return m_size;
}

void window::set_visibility(window::visibility is_visible) noexcept {
    m_is_visible = is_visible;
}

window::visibility window::get_visibility() const noexcept {
    return m_is_visible;
}

void window::set_focus(window::focus has_focus) noexcept {
    m_has_focus = has_focus;
}

window::focus window::get_focus() const noexcept {
    return m_has_focus;
}

void window::set_cursor_mode(window::cursor_mode mode) noexcept {
    m_cursor_mode = mode;
}

window::cursor_mode window::get_cursor_mode() const noexcept {
    return m_cursor_mode;
}

void window::set_fullscreen_mode(window::fullscreen_mode mode) noexcept {
    m_fullscreen_mode = mode;
}

window::fullscreen_mode window::get_fullscreen_mode() const noexcept {
    return m_fullscreen_mode;
}

void window::set_state(window::state window_state) noexcept {
    m_window_state = window_state;
}

window::state window::get_state() const noexcept {
    return m_window_state;
}

} // namespace tst::application
