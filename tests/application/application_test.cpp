#include <application/application.h>
#include <application/arg_parser.h>
#include <device/monitor.h>
#include <gtest/gtest.h>

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

TEST(application, constructor_creates_main_window) {
    glfw_guard glfw;
    if (!glfw.initialized()) {
        GTEST_SKIP()
            << "GLFW initialization failed in test environment. error_code=" << glfw.error_code()
            << " description=" << (glfw.error_description() != nullptr ? glfw.error_description() : "no description");
    }

    device::monitor primary_monitor(glfwGetPrimaryMonitor());
    [[maybe_unused]] application test_application(parse_program_arguments(0, nullptr), primary_monitor);
    SUCCEED();
}

TEST(main_window, uses_caller_supplied_name_and_extent) {
    glfw_guard glfw;
    if (!glfw.initialized()) {
        GTEST_SKIP()
            << "GLFW initialization failed in test environment. error_code=" << glfw.error_code()
            << " description=" << (glfw.error_description() != nullptr ? glfw.error_description() : "no description");
    }

    device::monitor primary_monitor(glfwGetPrimaryMonitor());
    event_processor<event> events;
    const auto supplied_size = core::extent<int32_t>{640, 360};
    main_window test_window(
        "Caller supplied name", supplied_size, primary_monitor, window::fullscreen_mode::windowed, events);

    EXPECT_EQ(test_window.get_name(), "Caller supplied name");
    EXPECT_EQ(test_window.get_fullscreen_mode(), window::fullscreen_mode::windowed);
    EXPECT_EQ(test_window.get_size(), supplied_size);
}

TEST(application, DISABLED_run_returns_when_main_window_should_close) {
    glfw_guard glfw;
    if (!glfw.initialized()) {
        GTEST_SKIP()
            << "GLFW initialization failed in test environment. error_code=" << glfw.error_code()
            << " description=" << (glfw.error_description() != nullptr ? glfw.error_description() : "no description");
    }

    device::monitor primary_monitor(glfwGetPrimaryMonitor());
    application test_application(parse_program_arguments(0, nullptr), primary_monitor);

    test_application.run();

    SUCCEED();
}

} // namespace tst::application
