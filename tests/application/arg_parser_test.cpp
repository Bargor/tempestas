#include <application/arg_parser.h>
#include <gtest/gtest.h>

namespace tst::application {

TEST(arg_parser, uses_windowed_mode_by_default) {
    const auto params = parse_program_arguments(0, nullptr);

    EXPECT_EQ(params.fullscreen_mode, window::fullscreen_mode::windowed);
}

TEST(arg_parser, parses_short_fullscreen_option) {
    char program_name[] = "tempestas";
    char fullscreen_option[] = "-f";
    char* arguments[]{program_name, fullscreen_option};

    const auto params = parse_program_arguments(2, arguments);

    EXPECT_EQ(params.fullscreen_mode, window::fullscreen_mode::fullscreen);
}

TEST(arg_parser, parses_long_fullscreen_option) {
    char program_name[] = "tempestas";
    char fullscreen_option[] = "--fullscreen";
    char* arguments[]{program_name, fullscreen_option};

    const auto params = parse_program_arguments(2, arguments);

    EXPECT_EQ(params.fullscreen_mode, window::fullscreen_mode::fullscreen);
}

TEST(arg_parser, parses_short_windowed_option) {
    char program_name[] = "tempestas";
    char fullscreen_option[] = "--fullscreen";
    char windowed_option[] = "-w";
    char* arguments[]{program_name, fullscreen_option, windowed_option};

    const auto params = parse_program_arguments(3, arguments);

    EXPECT_EQ(params.fullscreen_mode, window::fullscreen_mode::windowed);
}

TEST(arg_parser, parses_long_windowed_option) {
    char program_name[] = "tempestas";
    char fullscreen_option[] = "-f";
    char windowed_option[] = "--windowed";
    char* arguments[]{program_name, fullscreen_option, windowed_option};

    const auto params = parse_program_arguments(3, arguments);

    EXPECT_EQ(params.fullscreen_mode, window::fullscreen_mode::windowed);
}

} // namespace tst::application
