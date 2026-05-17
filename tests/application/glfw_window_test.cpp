#include <application/glfw_window.h>
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

TEST(glfw_window, creates_window_in_windowed_mode_without_monitor) {
    glfw_guard glfw;
    if (!glfw.initialized()) {
        GTEST_SKIP()
            << "GLFW initialization failed in test environment. error_code=" << glfw.error_code()
            << " description=" << (glfw.error_description() != nullptr ? glfw.error_description() : "no description");
    }

    const auto hints = glfw_context();

    glfw_window test_window("GLFW test",
                            core::extent<int32_t>{800, 600},
                            nullptr,
                            window::visibility_mode::hidden,
                            window::focus_mode::unfocused,
                            window::cursor_mode::normal,
                            window::fullscreen_mode::windowed,
                            window::window_display_state::opened,
                            hints);

    EXPECT_NE(test_window.get_handle(), nullptr);
    EXPECT_EQ(test_window.get_monitor(), nullptr);
    EXPECT_EQ(test_window.get_visibility(), window::visibility_mode::hidden);
    EXPECT_EQ(test_window.get_focus(), window::focus_mode::unfocused);
    EXPECT_EQ(test_window.get_state(), window::window_display_state::opened);
}

} // namespace tst::application
