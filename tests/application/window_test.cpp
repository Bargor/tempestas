#include <application/window.h>
#include <gtest/gtest.h>

namespace tst::application {

namespace {

    class test_window final : public window {
    public:
        using window::window;

        void close() noexcept override {
            m_closed = true;
        }

        bool is_closed() const noexcept {
            return m_closed;
        }

    private:
        bool m_closed{};
    };

} // namespace

TEST(window, constructor_assigns_all_properties) {
    test_window test_window("Main",
                            core::extent<int32_t>{1280, 720},
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::disabled,
                            window::fullscreen_mode::fullscreen,
                            window::window_display_state::maximized);

    EXPECT_EQ(test_window.get_name(), "Main");
    EXPECT_EQ(test_window.get_size(), (core::extent<int32_t>{1280, 720}));
    EXPECT_EQ(test_window.get_visibility(), window::visibility_mode::hidden);
    EXPECT_EQ(test_window.get_focus(), window::focus_mode::unfocused);
    EXPECT_EQ(test_window.get_cursor_mode(), window::cursor_mode::disabled);
    EXPECT_EQ(test_window.get_fullscreen_mode(), window::fullscreen_mode::fullscreen);
    EXPECT_EQ(test_window.get_state(), window::window_display_state::maximized);
}

TEST(window, setters_update_properties) {
    test_window test_window("Main", core::extent<int32_t>{640, 480});

    test_window.set_name("Editor");
    test_window.set_size({1920, 1080});
    test_window.set_visibility(window::visibility_mode::hidden);
    test_window.set_focus(window::focus_mode::unfocused);
    test_window.set_cursor_mode(window::cursor_mode::hidden);
    test_window.set_fullscreen_mode(window::fullscreen_mode::fullscreen);
    test_window.set_state(window::window_display_state::iconified);

    EXPECT_EQ(test_window.get_name(), "Editor");
    EXPECT_EQ(test_window.get_size(), (core::extent<int32_t>{1920, 1080}));
    EXPECT_EQ(test_window.get_visibility(), window::visibility_mode::hidden);
    EXPECT_EQ(test_window.get_focus(), window::focus_mode::unfocused);
    EXPECT_EQ(test_window.get_cursor_mode(), window::cursor_mode::hidden);
    EXPECT_EQ(test_window.get_fullscreen_mode(), window::fullscreen_mode::fullscreen);
    EXPECT_EQ(test_window.get_state(), window::window_display_state::iconified);
}

TEST(window, close_uses_derived_window_implementation) {
    test_window test_window("Main", core::extent<int32_t>{640, 480});

    window& base_window = test_window;
    base_window.close();

    EXPECT_TRUE(test_window.is_closed());
}

} // namespace tst::application
