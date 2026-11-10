#include <compare>
#include <core/util/flags.h>
#include <cstdint>
#include <gtest/gtest.h>
#include <type_traits>

using namespace tst::core;

enum class test_flag : std::uint8_t {
    none = 0x00,
    read = 0x01,
    write = 0x02,
    execute = 0x04,
};

enum class default_traits_flag : std::uint8_t {
    first = 0x01,
    second = 0x02,
};

namespace tst::core {

template<>
struct flag_traits<test_flag> {
    enum { all_flags = 0x07 };
};

} // namespace tst::core

using flag_type = test_flag;
using flags_type = tst::core::flags<flag_type>;
using mask_type = flags_type::MaskType;
using default_traits_flags_type = tst::core::flags<default_traits_flag>;

static_assert(std::is_same_v<mask_type, std::uint8_t>);
static_assert(std::is_constructible_v<flags_type, mask_type>);
static_assert(!std::is_convertible_v<mask_type, flags_type>);
static_assert(std::is_convertible_v<flag_type, flags_type>);
static_assert(!std::is_convertible_v<flags_type, bool>);
static_assert(!std::is_convertible_v<flags_type, mask_type>);

static_assert([] {
    constexpr flags_type read{flag_type::read};
    constexpr flags_type write{flag_type::write};
    constexpr auto read_write = read | write;
    constexpr auto remaining = ~read;

    return static_cast<mask_type>(read_write) ==
        0x03 &&
        remaining == (flags_type{flag_type::write} | flags_type{flag_type::execute});
}());

TEST(flags_test, default_constructs_to_zero) {
    const flags_type value{};

    EXPECT_TRUE(!value);
    EXPECT_FALSE(static_cast<bool>(value));
    EXPECT_EQ(static_cast<mask_type>(value), mask_type{0x00});
}

TEST(flags_test, constructs_from_a_single_bit) {
    const flags_type value{flag_type::read};

    EXPECT_TRUE(static_cast<bool>(value));
    EXPECT_EQ(static_cast<mask_type>(value), mask_type{0x01});
}

TEST(flags_test, supports_implicit_construction_from_the_enum) {
    const flags_type value = flag_type::write;

    EXPECT_EQ(static_cast<mask_type>(value), mask_type{0x02});
}

TEST(flags_test, constructs_from_an_explicit_mask) {
    const flags_type value{mask_type{0x05}};

    EXPECT_EQ(static_cast<mask_type>(value), mask_type{0x05});
    EXPECT_TRUE(static_cast<bool>(value));
}

TEST(flags_test, copy_constructs) {
    const flags_type original = flags_type{flag_type::read} | flags_type{flag_type::execute};
    const flags_type copy{original};

    EXPECT_EQ(copy, original);
    EXPECT_EQ(static_cast<mask_type>(copy), mask_type{0x05});
}

TEST(flags_test, copy_assigns) {
    const flags_type source = flags_type{flag_type::read} | flags_type{flag_type::write};
    flags_type destination{flag_type::execute};

    destination = source;

    EXPECT_EQ(destination, source);
    EXPECT_EQ(static_cast<mask_type>(destination), mask_type{0x03});
}

TEST(flags_test, logical_not_reports_whether_the_set_is_empty) {
    EXPECT_TRUE(!flags_type{});
    EXPECT_FALSE(!flags_type{flag_type::read});
}

TEST(flags_test, bitwise_and_returns_the_common_bits) {
    const flags_type read_write = flags_type{flag_type::read} | flags_type{flag_type::write};
    const flags_type write_execute = flags_type{flag_type::write} | flags_type{flag_type::execute};

    EXPECT_EQ(read_write & write_execute, flags_type{flag_type::write});
    EXPECT_EQ(read_write & flags_type{}, flags_type{});
}

TEST(flags_test, bitwise_or_returns_the_union_of_bits) {
    const flags_type read = flag_type::read;
    const flags_type write = flag_type::write;

    EXPECT_EQ(read | write, flags_type{mask_type{0x03}});
    EXPECT_EQ(read | flags_type{flag_type::read}, read);
}

TEST(flags_test, bitwise_xor_returns_the_differing_bits) {
    const flags_type read_write = flags_type{flag_type::read} | flags_type{flag_type::write};

    EXPECT_EQ(read_write ^ flags_type{flag_type::write}, flags_type{flag_type::read});
    EXPECT_EQ(read_write ^ read_write, flags_type{});
}

TEST(flags_test, bitwise_not_is_limited_to_the_traits_mask) {
    const flags_type read{flag_type::read};
    const flags_type expected = flags_type{flag_type::write} | flags_type{flag_type::execute};

    EXPECT_EQ(~read, expected);
    EXPECT_EQ(~flags_type{}, flags_type{mask_type{0x07}});
}

TEST(flags_test, bitwise_not_with_default_traits_returns_no_flags) {
    flags<default_traits_flag> value(default_traits_flag::first);

    EXPECT_EQ(~value, flags<default_traits_flag>{});
}

TEST(flags_test, bitwise_and_assignment_updates_the_mask) {
    flags_type value = flags_type{flag_type::read} | flags_type{flag_type::write} | flags_type{flag_type::execute};

    value &= flags_type{flag_type::read} | flags_type{flag_type::execute};

    EXPECT_EQ(value, flags_type{mask_type{0x05}});
}

TEST(flags_test, bitwise_or_assignment_updates_the_mask) {
    flags_type value{flag_type::read};

    value |= flags_type{flag_type::write};

    EXPECT_EQ(value, flags_type{mask_type{0x03}});
}

TEST(flags_test, bitwise_xor_assignment_updates_the_mask) {
    flags_type value = flags_type{flag_type::read} | flags_type{flag_type::write};

    value ^= flags_type{flag_type::write} | flags_type{flag_type::execute};

    EXPECT_EQ(value, flags_type{mask_type{0x05}});
}

TEST(flags_test, bitwise_operators_accept_a_flag_on_the_left_hand_side) {
    const flags_type read_write = flags_type{flag_type::read} | flags_type{flag_type::write};

    EXPECT_EQ(flag_type::read & read_write, flags_type{flag_type::read});
    EXPECT_EQ(flag_type::execute | read_write, flags_type{mask_type{0x07}});
    EXPECT_EQ(flag_type::read ^ read_write, flags_type{flag_type::write});
}

TEST(flags_test, supports_explicit_conversion_to_bool) {
    EXPECT_FALSE(static_cast<bool>(flags_type{}));
    EXPECT_TRUE(static_cast<bool>(flags_type{flag_type::execute}));
}

TEST(flags_test, supports_explicit_conversion_to_the_underlying_mask_type) {
    const flags_type value = flags_type{flag_type::read} | flags_type{flag_type::execute};

    EXPECT_EQ(static_cast<mask_type>(value), mask_type{0x05});
}

TEST(flags_test, compares_for_equality) {
    const flags_type read_a{flag_type::read};
    const flags_type read_b{flag_type::read};

    EXPECT_EQ(read_a, read_b);
    EXPECT_NE(read_a, flags_type{flag_type::write});
}

TEST(flags_test, supports_ordering_comparisons) {
    const flags_type empty{};
    const flags_type read{flag_type::read};
    const flags_type write{flag_type::write};

    EXPECT_LT(empty, read);
    EXPECT_LE(read, read);
    EXPECT_GT(write, read);
    EXPECT_GE(write, write);
}
