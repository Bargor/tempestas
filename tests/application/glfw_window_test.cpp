#include <application/glfw_window.h>
#include <application/input_processor.h>
#include <gtest/gtest.h>
#include <vector>

namespace tst::application {

namespace {
    class glfw_guard {
    public:
        glfw_guard() : m_initialized(glfwInit() == GLFW_TRUE) {
            if (!m_initialized) {
                m_error_code = glfwGetError(&m_error_description);
            }
        }

        ~glfw_guard() {
            if (m_initialized) {
                glfwTerminate();
            }
        }

        bool initialized() const noexcept {
            return m_initialized;
        }

        int error_code() const noexcept {
            return m_error_code;
        }

        const char* error_description() const noexcept {
            return m_error_description;
        }

    private:
        bool m_initialized{false};
        int m_error_code{GLFW_NO_ERROR};
        const char* m_error_description{nullptr};
    };
} // namespace

TEST(glfw_window, creates_window_in_windowed_mode_without_monitor) {
    glfw_guard glfw;
    if (!glfw.initialized()) {
        GTEST_SKIP()
            << "GLFW initialization failed in test environment. error_code=" << glfw.error_code()
            << " description=" << (glfw.error_description() != nullptr ? glfw.error_description() : "no description");
    }

    const auto hints = glfw_context();
    event_processor<event> events;

    glfw_window test_window("GLFW test",
                            core::extent<int32_t>{800, 600},
                            nullptr,
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::normal,
                            window::fullscreen_mode::windowed,
                            window::window_display_state::opened,
                            hints,
                            events);

    EXPECT_NE(test_window.get_handle(), nullptr);
    EXPECT_EQ(test_window.get_monitor(), nullptr);
    EXPECT_EQ(test_window.get_visibility(), window::visibility_mode::hidden);
    EXPECT_EQ(test_window.get_focus(), window::focus_mode::unfocused);
    EXPECT_EQ(test_window.get_state(), window::window_display_state::opened);
}

TEST(glfw_window, externally_sourced_events_update_internal_state) {
    glfw_guard glfw;
    if (!glfw.initialized()) GTEST_SKIP() << "GLFW initialization failed";

    event_processor<event> events;
    glfw_window test_window("GLFW test",
                            core::extent<int32_t>{800, 600},
                            nullptr,
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::normal,
                            window::fullscreen_mode::windowed,
                            window::window_display_state::opened,
                            glfw_context(),
                            events);
    int publisher{};

    ASSERT_TRUE(events.create_event(event::visibility{window::visibility_mode::visible}, &publisher));
    ASSERT_TRUE(events.create_event(event::framebuffer_size{{1024, 768}}, &publisher));
    ASSERT_TRUE(events.create_event(event::focus{window::focus_mode::focused}, &publisher));
    ASSERT_TRUE(events.create_event(event::iconify{window::window_display_state::iconified}, &publisher));
    ASSERT_TRUE(events.create_event(event::cursor_mode{window::cursor_mode::hidden}, &publisher));
    ASSERT_TRUE(events.create_event(event::closed{}, &publisher));

    events.process_events(event_processor<event>::duration::zero());

    EXPECT_EQ(test_window.get_size(), (core::extent<int32_t>{1024, 768}));
    EXPECT_EQ(test_window.get_focus(), window::focus_mode::focused);
    EXPECT_EQ(test_window.get_visibility(), window::visibility_mode::visible);
    EXPECT_EQ(test_window.get_state(), window::window_display_state::iconified);
    EXPECT_EQ(test_window.get_cursor_mode(), window::cursor_mode::hidden);
    EXPECT_EQ(glfwGetInputMode(test_window.get_handle(), GLFW_CURSOR), GLFW_CURSOR_NORMAL);
    EXPECT_EQ(glfwWindowShouldClose(test_window.get_handle()), GLFW_TRUE);
}

TEST(glfw_window, window_api_notifies_listeners_and_updates_glfw_state) {
    glfw_guard glfw;
    if (!glfw.initialized()) GTEST_SKIP() << "GLFW initialization failed";

    auto hints = glfw_context();
    for (auto& hint : hints) {
        if (hint.target == GLFW_VISIBLE || hint.target == GLFW_FOCUSED || hint.target == GLFW_FOCUS_ON_SHOW)
            hint.value = GLFW_FALSE;
    }

    event_processor<event> events;
    glfw_window test_window("GLFW test",
                            core::extent<int32_t>{800, 600},
                            nullptr,
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::normal,
                            window::fullscreen_mode::windowed,
                            window::window_display_state::opened,
                            hints,
                            events);
    input_processor input(test_window, events);
    std::vector<int> received;
    const auto update_frequency = event_processor<event>::duration::zero();
    events.subscribe<event::framebuffer_size>(
        nullptr,
        [&received](const event::framebuffer_size& value) {
            EXPECT_EQ(value.size, (core::extent<int32_t>{900, 700}));
            received.push_back(0);
        },
        update_frequency);
    events.subscribe<event::visibility>(
        nullptr,
        [&received](const event::visibility& value) {
            EXPECT_EQ(value.mode, window::visibility_mode::visible);
            received.push_back(1);
        },
        update_frequency);
    events.subscribe<event::focus>(
        nullptr,
        [&received](const event::focus& value) {
            EXPECT_EQ(value.mode, window::focus_mode::focused);
            received.push_back(2);
        },
        update_frequency);
    events.subscribe<event::cursor_mode>(
        nullptr,
        [&received](const event::cursor_mode& value) {
            EXPECT_EQ(value.mode, window::cursor_mode::hidden);
            received.push_back(3);
        },
        update_frequency);
    events.subscribe<event::closed>(
        nullptr, [&received](const event::closed&) { received.push_back(4); }, update_frequency);

    window& window_api = test_window;
    window_api.set_size({900, 700});
    auto framebuffer_size_callback = glfwSetFramebufferSizeCallback(test_window.get_handle(), nullptr);
    ASSERT_NE(framebuffer_size_callback, nullptr);
    glfwSetFramebufferSizeCallback(test_window.get_handle(), framebuffer_size_callback);
    framebuffer_size_callback(test_window.get_handle(), 900, 700);
    events.process_events(event_processor<event>::duration::zero());

    window_api.set_visibility(window::visibility_mode::visible);
    EXPECT_EQ(glfwGetWindowAttrib(test_window.get_handle(), GLFW_VISIBLE), GLFW_TRUE);
    EXPECT_EQ(test_window.get_visibility(), window::visibility_mode::visible);
    events.process_events(event_processor<event>::duration::zero());

    window_api.set_focus(window::focus_mode::focused);
    auto focus_callback = glfwSetWindowFocusCallback(test_window.get_handle(), nullptr);
    ASSERT_NE(focus_callback, nullptr);
    glfwSetWindowFocusCallback(test_window.get_handle(), focus_callback);
    focus_callback(test_window.get_handle(), GLFW_TRUE);
    events.process_events(event_processor<event>::duration::zero());

    window_api.set_cursor_mode(window::cursor_mode::hidden);
    EXPECT_EQ(glfwGetInputMode(test_window.get_handle(), GLFW_CURSOR), GLFW_CURSOR_HIDDEN);
    events.process_events(event_processor<event>::duration::zero());

    window_api.close();
    EXPECT_EQ(glfwWindowShouldClose(test_window.get_handle()), GLFW_TRUE);
    events.process_events(event_processor<event>::duration::zero());

    ASSERT_EQ(received.size(), 5);
    EXPECT_EQ(received, (std::vector<int>{0, 1, 2, 3, 4}));
    EXPECT_EQ(test_window.get_size(), (core::extent<int32_t>{900, 700}));
    EXPECT_EQ(test_window.get_visibility(), window::visibility_mode::visible);
    EXPECT_EQ(test_window.get_focus(), window::focus_mode::focused);
    EXPECT_EQ(test_window.get_cursor_mode(), window::cursor_mode::hidden);
    EXPECT_EQ(glfwGetWindowAttrib(test_window.get_handle(), GLFW_FOCUSED), GLFW_TRUE);
    EXPECT_EQ(glfwWindowShouldClose(test_window.get_handle()), GLFW_TRUE);
}

} // namespace tst::application
