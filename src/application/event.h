#pragma once

#include <application/window.h>
#include <chrono>
#include <cstdint>
#include <device/keyboard.h>
#include <device/mouse.h>
#include <type_traits>
#include <variant>

namespace tst::application {

struct event {
    struct mouse_position {
        double x{};
        double y{};
    };

    struct mouse_button {
        device::mouse_button button{};
        device::mouse_action action{};
        int32_t mods{};
    };

    struct scroll {
        double x_offset{};
        double y_offset{};
    };

    struct keyboard {
        device::keys key{};
        int32_t scancode{};
        device::key_action action{};
        int32_t mods{};
    };

    struct iconify {
        window::window_display_state state{};
    };

    struct focus {
        window::focus_mode mode{};
    };

    struct framebuffer_size {
        core::extent<int32_t> size{};
    };

    struct closed {};

    struct visibility {
        window::visibility_mode mode{};
    };

    struct cursor_mode {
        window::cursor_mode mode{};
    };

    struct time {
        std::chrono::microseconds value{};
    };

    using payload =
        std::variant<mouse_position, mouse_button, scroll, keyboard, iconify, focus, framebuffer_size, closed, visibility, cursor_mode, time>;

    // The object that created the event. Matching subscribers are skipped during dispatch.
    const void* source{};
#if defined(__clang__) && __clang_major__ == 21
    // Clang 21 incorrectly rejects the variant default constructor for a nested first alternative while event is
    // incomplete.
    payload data{mouse_position{}};
#else
    payload data{};
#endif
    // Assigned by event_processor when the event is queued. Zero means not queued.
    uint64_t event_id{};
};

static_assert(std::is_default_constructible_v<event>, "event must be default-constructible");
static_assert(std::is_trivially_copy_constructible_v<event>, "event must be trivially copy-constructible");
static_assert(std::is_trivially_copy_assignable_v<event>, "event must be trivially copy-assignable");

} // namespace tst::application
