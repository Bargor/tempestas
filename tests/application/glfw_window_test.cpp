#include <application/glfw_window.h>
#include <gtest/gtest.h>

namespace tst::application {

namespace {
    class glfw_guard {
    public:
        glfw_guard() : m_initialized(glfwInit() == GLFW_TRUE) {
        }

        ~glfw_guard() {
            if (m_initialized) {
                glfwTerminate();
            }
        }

        bool initialized() const noexcept {
            return m_initialized;
        }

    private:
        bool m_initialized{false};
    };
} // namespace

TEST(glfw_window, creates_window_and_keeps_monitor_reference) {
    glfw_guard glfw;
    if (!glfw.initialized()) {
        GTEST_SKIP() << "GLFW initialization failed in test environment.";
    }

    GLFWmonitor* primary_handle = glfwGetPrimaryMonitor();
    if (primary_handle == nullptr) {
        GTEST_SKIP() << "Primary monitor is unavailable in test environment.";
    }

    const device::monitor primary_monitor(primary_handle);

    glfw_window test_window("GLFW test",
                            core::extent<int32_t>{800, 600},
                            primary_monitor,
                            window::visibility::hidden,
                            window::focus::unfocused,
                            window::cursor_mode::normal,
                            window::fullscreen_mode::windowed,
                            window::state::opened);

    EXPECT_NE(test_window.get_handle(), nullptr);
    EXPECT_EQ(test_window.get_monitor().get_handle(), primary_handle);
    EXPECT_EQ(test_window.get_visibility(), window::visibility::hidden);
    EXPECT_EQ(test_window.get_focus(), window::focus::unfocused);
    EXPECT_EQ(test_window.get_state(), window::state::opened);
}

} // namespace tst::application
