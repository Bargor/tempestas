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

    glfw_window test_window("Input test",
                            {800, 600},
                            nullptr,
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::normal,
                            window::fullscreen_mode::windowed,
                            window::window_display_state::opened,
                            glfw_context());
    event_processor<event> events;
    std::vector<event> received;
    const auto collect = [&received](const event& value) { received.push_back(value); };
    events.subscribe<event::focus>(collect);
    events.subscribe<event::mouse_position>(collect);
    events.subscribe<event::mouse_button>(collect);
    events.subscribe<event::scroll>(collect);
    events.subscribe<event::keyboard>(collect);
    events.subscribe<event::iconify>(collect);
    events.subscribe<event::closed>(collect);
    events.subscribe<event::framebuffer_size>(collect);

    {
        input_processor input(test_window, events);
        GLFWwindow* handle = test_window.get_handle();

        auto focus_callback = glfwSetWindowFocusCallback(handle, nullptr);
        ASSERT_NE(focus_callback, nullptr);
        glfwSetWindowFocusCallback(handle, focus_callback);
        focus_callback(handle, GLFW_TRUE);

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
        iconify_callback(handle, GLFW_FALSE);

        auto close_callback = glfwSetWindowCloseCallback(handle, nullptr);
        ASSERT_NE(close_callback, nullptr);
        glfwSetWindowCloseCallback(handle, close_callback);
        close_callback(handle);

        auto size_callback = glfwSetFramebufferSizeCallback(handle, nullptr);
        ASSERT_NE(size_callback, nullptr);
        glfwSetFramebufferSizeCallback(handle, size_callback);
        size_callback(handle, 1024, 768);

        events.process_events(event_processor<event>::duration::zero());
    }

    ASSERT_EQ(received.size(), 9);
    for (const auto& value : received)
        EXPECT_EQ(value.source, &test_window);

    const auto* focus = std::get_if<event::focus>(&received[0].data);
    ASSERT_NE(focus, nullptr);
    EXPECT_EQ(focus->mode, window::focus_mode::focused);

    const auto* position = std::get_if<event::mouse_position>(&received[1].data);
    ASSERT_NE(position, nullptr);
    EXPECT_DOUBLE_EQ(position->x, 12.5);
    EXPECT_DOUBLE_EQ(position->y, 24.5);

    const auto* button = std::get_if<event::mouse_button>(&received[2].data);
    ASSERT_NE(button, nullptr);
    EXPECT_EQ(button->button, device::mouse_button::button_left);
    EXPECT_EQ(button->action, device::mouse_action::press);
    EXPECT_EQ(button->mods, GLFW_MOD_SHIFT);

    const auto* scroll = std::get_if<event::scroll>(&received[3].data);
    ASSERT_NE(scroll, nullptr);
    EXPECT_DOUBLE_EQ(scroll->x_offset, -1.0);
    EXPECT_DOUBLE_EQ(scroll->y_offset, 2.0);

    const auto* key = std::get_if<event::keyboard>(&received[4].data);
    ASSERT_NE(key, nullptr);
    EXPECT_EQ(key->key, device::keys::key_a);
    EXPECT_EQ(key->scancode, 17);
    EXPECT_EQ(key->action, device::key_action::repeat);
    EXPECT_EQ(key->mods, GLFW_MOD_CONTROL);

    const auto* iconified = std::get_if<event::iconify>(&received[5].data);
    ASSERT_NE(iconified, nullptr);
    EXPECT_EQ(iconified->state, window::window_display_state::iconified);
    const auto* restored = std::get_if<event::iconify>(&received[6].data);
    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(restored->state, window::window_display_state::opened);

    EXPECT_NE(std::get_if<event::closed>(&received[7].data), nullptr);

    const auto* size = std::get_if<event::framebuffer_size>(&received[8].data);
    ASSERT_NE(size, nullptr);
    EXPECT_EQ(size->size.width, 1024);
    EXPECT_EQ(size->size.height, 768);

    EXPECT_EQ(glfwGetWindowUserPointer(test_window.get_handle()), nullptr);
    EXPECT_EQ(glfwSetWindowFocusCallback(test_window.get_handle(), nullptr), nullptr);
}

TEST(input_processor, process_events_polls_and_dispatches_queued_events) {
    glfw_guard glfw;
    if (!glfw.initialized()) GTEST_SKIP() << "GLFW initialization failed";

    glfw_window test_window("Input test",
                            {800, 600},
                            nullptr,
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::normal,
                            window::fullscreen_mode::windowed,
                            window::window_display_state::opened,
                            glfw_context());
    event_processor<event> events;
    input_processor input(test_window, events);
    int close_count = 0;
    events.subscribe<event::closed>([&close_count](const event&) { ++close_count; });

    ASSERT_TRUE(events.create_event(event{&test_window, event::closed{}}));
    input.process_events();

    EXPECT_EQ(close_count, 1);
}

TEST(input_processor_DeathTest, rejects_existing_glfw_callback) {
    GTEST_FLAG_SET(death_test_style, "threadsafe");

    EXPECT_DEATH(
        {
            glfw_guard glfw;
            if (!glfw.initialized()) std::_Exit(EXIT_SUCCESS);

            glfw_window test_window("Input test",
                                    {800, 600},
                                    nullptr,
                                    window::visibility_mode::hidden,
                                    window::focus_mode::unfocused,
                                    window::cursor_mode::normal,
                                    window::fullscreen_mode::windowed,
                                    window::window_display_state::opened,
                                    glfw_context());
            event_processor<event> events;
            if (glfwSetWindowFocusCallback(test_window.get_handle(), +[](GLFWwindow*, int) {}) != nullptr)
                std::_Exit(EXIT_SUCCESS);

            input_processor input(test_window, events);
        },
        "");
}

TEST(input_processor, cursor_position_uses_relative_coordinates_only_when_disabled) {
    glfw_guard glfw;
    if (!glfw.initialized()) GTEST_SKIP() << "GLFW initialization failed";

    glfw_window test_window("Input test",
                            {800, 600},
                            nullptr,
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::disabled,
                            window::fullscreen_mode::windowed,
                            window::window_display_state::opened,
                            glfw_context());
    event_processor<event> events;
    std::vector<event::mouse_position> positions;
    events.subscribe<event::mouse_position>([&positions](const event& value) {
        const auto* position = std::get_if<event::mouse_position>(&value.data);
        ASSERT_NE(position, nullptr);
        positions.push_back(*position);
    });

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
    position_callback(handle, 70.0, 80.0);

    test_window.set_cursor_mode(window::cursor_mode::normal);
    EXPECT_EQ(glfwGetInputMode(handle, GLFW_CURSOR), GLFW_CURSOR_NORMAL);
    position_callback(handle, 90.0, 100.0);

    test_window.window::set_cursor_mode(window::cursor_mode::disabled);
    EXPECT_EQ(glfwGetInputMode(handle, GLFW_CURSOR), GLFW_CURSOR_NORMAL);
    position_callback(handle, 94.0, 107.0);
    test_window.set_cursor_mode(window::cursor_mode::normal);

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
