#include <application/window.h>
#include <gtest/gtest.h>

namespace tst::application {

TEST(window, constructor_assigns_all_properties) {
    window test_window("Main",
                       core::extent<int32_t>{1280, 720},
                       window::visibility_mode::hidden,
                       window::focus_mode::unfocused,
                       window::cursor_mode::disabled,
                       window::fullscreen_mode::fullscreen,
                       window::state_mode::maximized);

    EXPECT_EQ(test_window.get_name(), "Main");
    EXPECT_EQ(test_window.get_size(), (core::extent<int32_t>{1280, 720}));
    EXPECT_EQ(test_window.get_visibility(), window::visibility_mode::hidden);
    EXPECT_EQ(test_window.get_focus(), window::focus_mode::unfocused);
    EXPECT_EQ(test_window.get_cursor_mode(), window::cursor_mode::disabled);
    EXPECT_EQ(test_window.get_fullscreen_mode(), window::fullscreen_mode::fullscreen);
    EXPECT_EQ(test_window.get_state(), window::state_mode::maximized);
}

TEST(window, setters_update_properties) {
    window test_window("Main", core::extent<int32_t>{640, 480});

    test_window.set_name("Editor");
    test_window.set_size({1920, 1080});
    test_window.set_visibility(window::visibility_mode::hidden);
    test_window.set_focus(window::focus_mode::unfocused);
    test_window.set_cursor_mode(window::cursor_mode::hidden);
    test_window.set_fullscreen_mode(window::fullscreen_mode::fullscreen);
    test_window.set_state(window::state_mode::iconified);

    EXPECT_EQ(test_window.get_name(), "Editor");
    EXPECT_EQ(test_window.get_size(), (core::extent<int32_t>{1920, 1080}));
    EXPECT_EQ(test_window.get_visibility(), window::visibility_mode::hidden);
    EXPECT_EQ(test_window.get_focus(), window::focus_mode::unfocused);
    EXPECT_EQ(test_window.get_cursor_mode(), window::cursor_mode::hidden);
    EXPECT_EQ(test_window.get_fullscreen_mode(), window::fullscreen_mode::fullscreen);
    EXPECT_EQ(test_window.get_state(), window::state_mode::iconified);
}

} // namespace tst::application
