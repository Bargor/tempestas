#include <application/event.h>
#include <application/event_processor.h>
#include <application/glfw_context.h>
#include <application/glfw_window.h>
#include <application/input_processor.h>
#include <cstdlib>
#include <gtest/gtest.h>
#include <vector>

namespace tst::application {

namespace {

    class glfw_guard {
    public:
        glfw_guard() : m_initialized(glfwInit() == GLFW_TRUE) {
        }

        ~glfw_guard() {
            if (m_initialized) glfwTerminate();
        }

        bool initialized() const noexcept {
            return m_initialized;
        }

    private:
        bool m_initialized{};
    };

} // namespace

TEST(input_processor, glfw_callbacks_queue_matching_events) {
    glfw_guard glfw;
    if (!glfw.initialized()) GTEST_SKIP() << "GLFW initialization failed";

    event_processor<event> events;
    glfw_window test_window("Input test",
                            {800, 600},
                            nullptr,
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::normal,
                            window::fullscreen_mode::windowed,
                            window::window_display_state::opened,
                            glfw_context(),
                            events);
    std::vector<int> received_order;
    event::focus received_focus{};
    event::mouse_position received_position{};
    event::mouse_button received_button{};
    event::scroll received_scroll{};
    event::keyboard received_key{};
    std::vector<event::iconify> received_iconify;
    event::framebuffer_size received_size{};
    const auto update_frequency = event_processor<event>::duration::zero();
    events.subscribe<event::focus>(
        nullptr,
        [&](const event::focus& value) {
            received_focus = value;
            received_order.push_back(0);
        },
        update_frequency);
    events.subscribe<event::mouse_position>(
        nullptr,
        [&](const event::mouse_position& value) {
            received_position = value;
            received_order.push_back(1);
        },
        update_frequency);
    events.subscribe<event::mouse_button>(
        nullptr,
        [&](const event::mouse_button& value) {
            received_button = value;
            received_order.push_back(2);
        },
        update_frequency);
    events.subscribe<event::scroll>(
        nullptr,
        [&](const event::scroll& value) {
            received_scroll = value;
            received_order.push_back(3);
        },
        update_frequency);
    events.subscribe<event::keyboard>(
        nullptr,
        [&](const event::keyboard& value) {
            received_key = value;
            received_order.push_back(4);
        },
        update_frequency);
    events.subscribe<event::iconify>(
        nullptr,
        [&](const event::iconify& value) {
            received_iconify.push_back(value);
            received_order.push_back(5);
        },
        update_frequency);
    events.subscribe<event::closed>(nullptr, [&](const event::closed&) { received_order.push_back(6); }, update_frequency);
    events.subscribe<event::framebuffer_size>(
        nullptr,
        [&](const event::framebuffer_size& value) {
            received_size = value;
            received_order.push_back(7);
        },
        update_frequency);

    {
        input_processor input(test_window, events);
        GLFWwindow* handle = test_window.get_handle();

        auto focus_callback = glfwSetWindowFocusCallback(handle, nullptr);
        ASSERT_NE(focus_callback, nullptr);
        glfwSetWindowFocusCallback(handle, focus_callback);
        focus_callback(handle, GLFW_TRUE);
        EXPECT_EQ(test_window.get_focus(), window::focus_mode::focused);

        auto position_callback = glfwSetCursorPosCallback(handle, nullptr);
        ASSERT_NE(position_callback, nullptr);
        glfwSetCursorPosCallback(handle, position_callback);
        position_callback(handle, 12.5, 24.5);

        auto button_callback = glfwSetMouseButtonCallback(handle, nullptr);
        ASSERT_NE(button_callback, nullptr);
        glfwSetMouseButtonCallback(handle, button_callback);
        button_callback(handle, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, GLFW_MOD_SHIFT);

        auto scroll_callback = glfwSetScrollCallback(handle, nullptr);
        ASSERT_NE(scroll_callback, nullptr);
        glfwSetScrollCallback(handle, scroll_callback);
        scroll_callback(handle, -1.0, 2.0);

        auto key_callback = glfwSetKeyCallback(handle, nullptr);
        ASSERT_NE(key_callback, nullptr);
        glfwSetKeyCallback(handle, key_callback);
        key_callback(handle, GLFW_KEY_A, 17, GLFW_REPEAT, GLFW_MOD_CONTROL);

        auto iconify_callback = glfwSetWindowIconifyCallback(handle, nullptr);
        ASSERT_NE(iconify_callback, nullptr);
        glfwSetWindowIconifyCallback(handle, iconify_callback);
        iconify_callback(handle, GLFW_TRUE);
        EXPECT_EQ(test_window.get_state(), window::window_display_state::iconified);
        iconify_callback(handle, GLFW_FALSE);
        EXPECT_EQ(test_window.get_state(), window::window_display_state::opened);

        auto close_callback = glfwSetWindowCloseCallback(handle, nullptr);
        ASSERT_NE(close_callback, nullptr);
        glfwSetWindowCloseCallback(handle, close_callback);
        glfwSetWindowShouldClose(handle, GLFW_TRUE);
        close_callback(handle);
        EXPECT_EQ(glfwWindowShouldClose(handle), GLFW_TRUE);

        auto size_callback = glfwSetFramebufferSizeCallback(handle, nullptr);
        ASSERT_NE(size_callback, nullptr);
        glfwSetFramebufferSizeCallback(handle, size_callback);
        size_callback(handle, 1024, 768);
        EXPECT_EQ(test_window.get_size(), (core::extent<int32_t>{1024, 768}));

        events.process_events(event_processor<event>::duration::zero());
    }

    EXPECT_EQ(received_order, (std::vector<int>{0, 1, 2, 3, 4, 5, 5, 6, 7}));
    EXPECT_EQ(received_focus.mode, window::focus_mode::focused);
    EXPECT_DOUBLE_EQ(received_position.x, 12.5);
    EXPECT_DOUBLE_EQ(received_position.y, 24.5);
    EXPECT_EQ(received_button.button, device::mouse_button::button_left);
    EXPECT_EQ(received_button.action, device::mouse_action::press);
    EXPECT_EQ(received_button.mods, GLFW_MOD_SHIFT);
    EXPECT_DOUBLE_EQ(received_scroll.x_offset, -1.0);
    EXPECT_DOUBLE_EQ(received_scroll.y_offset, 2.0);
    EXPECT_EQ(received_key.key, device::keys::key_a);
    EXPECT_EQ(received_key.scancode, 17);
    EXPECT_EQ(received_key.action, device::key_action::repeat);
    EXPECT_EQ(received_key.mods, GLFW_MOD_CONTROL);
    ASSERT_EQ(received_iconify.size(), 2);
    EXPECT_EQ(received_iconify[0].state, window::window_display_state::iconified);
    EXPECT_EQ(received_iconify[1].state, window::window_display_state::opened);
    EXPECT_EQ(glfwWindowShouldClose(test_window.get_handle()), GLFW_TRUE);
    EXPECT_EQ(received_size.size.width, 1024);
    EXPECT_EQ(received_size.size.height, 768);

    EXPECT_EQ(glfwGetWindowUserPointer(test_window.get_handle()), nullptr);
    EXPECT_EQ(glfwSetWindowFocusCallback(test_window.get_handle(), nullptr), nullptr);
}

TEST(input_processor, process_events_polls_and_dispatches_queued_events) {
    glfw_guard glfw;
    if (!glfw.initialized()) GTEST_SKIP() << "GLFW initialization failed";

    event_processor<event> events;
    glfw_window test_window("Input test",
                            {800, 600},
                            nullptr,
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::normal,
                            window::fullscreen_mode::windowed,
                            window::window_display_state::opened,
                            glfw_context(),
                            events);
    input_processor input(test_window, events);
    int close_count = 0;
    events.subscribe<event::closed>(
        nullptr, [&close_count](const event::closed&) { ++close_count; }, event_processor<event>::duration::zero());

    ASSERT_TRUE(events.create_event(event::closed{}, &test_window));
    input.process_events();

    EXPECT_EQ(close_count, 1);
}

TEST(input_processor_DeathTest, rejects_existing_glfw_callback) {
    GTEST_FLAG_SET(death_test_style, "threadsafe");

    EXPECT_DEATH(
        {
            glfw_guard glfw;
            if (!glfw.initialized()) std::_Exit(EXIT_SUCCESS);

            event_processor<event> events;
            glfw_window test_window("Input test",
                                    {800, 600},
                                    nullptr,
                                    window::visibility_mode::hidden,
                                    window::focus_mode::unfocused,
                                    window::cursor_mode::normal,
                                    window::fullscreen_mode::windowed,
                                    window::window_display_state::opened,
                                    glfw_context(),
                                    events);
            if (glfwSetWindowFocusCallback(test_window.get_handle(), +[](GLFWwindow*, int) {}) != nullptr)
                std::_Exit(EXIT_SUCCESS);

            input_processor input(test_window, events);
        },
        "");
}

TEST(input_processor, cursor_position_uses_relative_coordinates_only_when_disabled) {
    glfw_guard glfw;
    if (!glfw.initialized()) GTEST_SKIP() << "GLFW initialization failed";

    event_processor<event> events;
    glfw_window test_window("Input test",
                            {800, 600},
                            nullptr,
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::disabled,
                            window::fullscreen_mode::windowed,
                            window::window_display_state::opened,
                            glfw_context(),
                            events);
    std::vector<event::mouse_position> positions;
    events.subscribe<event::mouse_position>(
        nullptr,
        [&positions](const event::mouse_position& position) { positions.push_back(position); },
        event_processor<event>::duration::zero());

    input_processor input(test_window, events);
    GLFWwindow* handle = test_window.get_handle();
    EXPECT_EQ(glfwGetInputMode(handle, GLFW_CURSOR), GLFW_CURSOR_DISABLED);

    double initial_x{};
    double initial_y{};
    glfwGetCursorPos(handle, &initial_x, &initial_y);

    auto position_callback = glfwSetCursorPosCallback(handle, nullptr);
    ASSERT_NE(position_callback, nullptr);
    glfwSetCursorPosCallback(handle, position_callback);
    position_callback(handle, initial_x + 5.0, initial_y - 3.0);
    position_callback(handle, initial_x + 8.0, initial_y + 2.0);

    test_window.set_cursor_mode(window::cursor_mode::hidden);
    EXPECT_EQ(glfwGetInputMode(handle, GLFW_CURSOR), GLFW_CURSOR_HIDDEN);
    events.process_events(event_processor<event>::duration::zero());
    position_callback(handle, 70.0, 80.0);

    test_window.set_cursor_mode(window::cursor_mode::normal);
    EXPECT_EQ(glfwGetInputMode(handle, GLFW_CURSOR), GLFW_CURSOR_NORMAL);
    events.process_events(event_processor<event>::duration::zero());
    position_callback(handle, 90.0, 100.0);

    test_window.window::set_cursor_mode(window::cursor_mode::disabled);
    EXPECT_EQ(glfwGetInputMode(handle, GLFW_CURSOR), GLFW_CURSOR_NORMAL);
    position_callback(handle, 94.0, 107.0);
    test_window.window::set_cursor_mode(window::cursor_mode::normal);

    events.process_events(event_processor<event>::duration::zero());

    ASSERT_EQ(positions.size(), 5);
    EXPECT_DOUBLE_EQ(positions[0].x, 5.0);
    EXPECT_DOUBLE_EQ(positions[0].y, -3.0);
    EXPECT_DOUBLE_EQ(positions[1].x, 3.0);
    EXPECT_DOUBLE_EQ(positions[1].y, 5.0);
    EXPECT_DOUBLE_EQ(positions[2].x, 70.0);
    EXPECT_DOUBLE_EQ(positions[2].y, 80.0);
    EXPECT_DOUBLE_EQ(positions[3].x, 90.0);
    EXPECT_DOUBLE_EQ(positions[3].y, 100.0);
    EXPECT_DOUBLE_EQ(positions[4].x, 4.0);
    EXPECT_DOUBLE_EQ(positions[4].y, 7.0);
}

} // namespace tst::application
