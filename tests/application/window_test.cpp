#include <application/window.h>
#include <gtest/gtest.h>

namespace tst::application {

TEST(window, constructor_assigns_all_properties) {
    window test_window("Main",
                       core::extent<int32_t>{1280, 720},
                       window::visibility::hidden,
                       window::focus::unfocused,
                       window::cursor_mode::disabled,
                       window::fullscreen_mode::fullscreen,
                       window::state::maximized);

    EXPECT_EQ(test_window.get_name(), "Main");
    EXPECT_EQ(test_window.get_size(), (core::extent<int32_t>{1280, 720}));
    EXPECT_EQ(test_window.get_visibility(), window::visibility::hidden);
    EXPECT_EQ(test_window.get_focus(), window::focus::unfocused);
    EXPECT_EQ(test_window.get_cursor_mode(), window::cursor_mode::disabled);
    EXPECT_EQ(test_window.get_fullscreen_mode(), window::fullscreen_mode::fullscreen);
    EXPECT_EQ(test_window.get_state(), window::state::maximized);
}

TEST(window, setters_update_properties) {
    window test_window("Main", core::extent<int32_t>{640, 480});

    test_window.set_name("Editor");
    test_window.set_size({1920, 1080});
    test_window.set_visibility(window::visibility::hidden);
    test_window.set_focus(window::focus::unfocused);
    test_window.set_cursor_mode(window::cursor_mode::hidden);
    test_window.set_fullscreen_mode(window::fullscreen_mode::fullscreen);
    test_window.set_state(window::state::iconified);

    EXPECT_EQ(test_window.get_name(), "Editor");
    EXPECT_EQ(test_window.get_size(), (core::extent<int32_t>{1920, 1080}));
    EXPECT_EQ(test_window.get_visibility(), window::visibility::hidden);
    EXPECT_EQ(test_window.get_focus(), window::focus::unfocused);
    EXPECT_EQ(test_window.get_cursor_mode(), window::cursor_mode::hidden);
    EXPECT_EQ(test_window.get_fullscreen_mode(), window::fullscreen_mode::fullscreen);
    EXPECT_EQ(test_window.get_state(), window::state::iconified);
}

} // namespace tst::application
