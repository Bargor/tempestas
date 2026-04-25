#pragma once

#include <core/rectangle.h>
#include <cstdint>
#include <string>

namespace tst::application {

class window {
public:
    enum class visibility {
        visible,
        hidden,
    };

    enum class focus {
        focused,
        unfocused,
    };

    enum class cursor_mode {
        normal,
        hidden,
        disabled,
    };

    enum class fullscreen_mode {
        windowed,
        fullscreen,
    };

    enum class state {
        opened,
        iconified,
        maximized,
    };

    window(std::string name,
           core::extent<int32_t> size,
           visibility is_visible = visibility::visible,
           focus has_focus = focus::focused,
           cursor_mode cursor = cursor_mode::normal,
           fullscreen_mode fullscreen = fullscreen_mode::windowed,
           state window_state = state::opened) noexcept;
    virtual ~window() = default;

    window(const window&) = delete;
    window& operator=(const window&) = delete;
    window(window&&) = default;
    window& operator=(window&&) = default;

    void set_name(std::string name) noexcept;
    const std::string& get_name() const noexcept;

    void set_size(core::extent<int32_t> size) noexcept;
    const core::extent<int32_t>& get_size() const noexcept;

    void set_visibility(visibility is_visible) noexcept;
    visibility get_visibility() const noexcept;

    void set_focus(focus has_focus) noexcept;
    focus get_focus() const noexcept;

    void set_cursor_mode(cursor_mode mode) noexcept;
    cursor_mode get_cursor_mode() const noexcept;

    void set_fullscreen_mode(fullscreen_mode mode) noexcept;
    fullscreen_mode get_fullscreen_mode() const noexcept;

    void set_state(state window_state) noexcept;
    state get_state() const noexcept;

private:
    std::string m_name;
    core::extent<int32_t> m_size{};
    visibility m_is_visible{visibility::visible};
    focus m_has_focus{focus::focused};
    cursor_mode m_cursor_mode{cursor_mode::normal};
    fullscreen_mode m_fullscreen_mode{fullscreen_mode::windowed};
    state m_window_state{state::opened};
};

} // namespace tst::application
