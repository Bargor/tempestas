#include <algorithm>
#include <array>
#include <container/enum_array.h>
#include <cstddef>
#include <gtest/gtest.h>
#include <type_traits>

using namespace tst::core;

enum class test_enum { first, second, third, count };

enum class another_enum { first };

using test_array = enum_array<test_enum, int, static_cast<std::size_t>(test_enum::count)>;

template<typename A, typename I>
concept IndexableWith = requires(A& array, I index) { array[index]; };

template<typename A, typename V>
concept FillableWith = requires(A& array, V value) { array.fill(value); };

static_assert(IndexableWith<test_array, test_enum>);
static_assert(!IndexableWith<test_array, int>);
static_assert(!IndexableWith<test_array, another_enum>);

static_assert(FillableWith<test_array, test_enum>);
static_assert(!FillableWith<test_array, int>);
static_assert(!FillableWith<test_array, another_enum>);

TEST(enum_array, default_construction) {
    test_array array{};

    EXPECT_EQ(array.size(), 3);
}

TEST(enum_array, enum_indexing) {
    test_array array{};

    array[test_enum::first] = 10;
    array[test_enum::second] = 20;
    array[test_enum::third] = 30;

    EXPECT_EQ(array[test_enum::first], 10);
    EXPECT_EQ(array[test_enum::second], 20);
    EXPECT_EQ(array[test_enum::third], 30);
}

TEST(enum_array, const_indexing) {
    test_array array{};
    array[test_enum::first] = 42;

    const test_array& constArray = array;

    EXPECT_EQ(constArray[test_enum::first], 42);
}

TEST(enum_array, indexing_returns_reference) {
    test_array array{};

    static_assert(std::is_same_v<decltype(array[test_enum::first]), int&>);
    static_assert(std::is_same_v<decltype(std::as_const(array)[test_enum::first]), const int&>);
}

TEST(enum_array, fill_with_enum) {
    test_array array{};

    array.fill(test_enum::second);

    for (const auto value : array) {
        EXPECT_EQ(value, static_cast<int>(test_enum::second));
    }
}

TEST(enum_array, standard_container_interface) {
    test_array array{};

    array[test_enum::first] = 10;
    array[test_enum::second] = 20;
    array[test_enum::third] = 30;

    EXPECT_EQ(array.size(), 3);
    EXPECT_FALSE(array.empty());

    EXPECT_EQ(array.front(), 10);
    EXPECT_EQ(array.back(), 30);

    EXPECT_EQ(array.data()[0], 10);
}

TEST(enum_array, supports_iterators) {
    test_array array{};

    array[test_enum::first] = 1;
    array[test_enum::second] = 2;
    array[test_enum::third] = 3;

    EXPECT_EQ(std::distance(array.begin(), array.end()), 3);

    EXPECT_TRUE(std::all_of(array.begin(), array.end(), [](int value) { return value > 0; }));
}

TEST(enum_array, supports_range_based_for) {
    test_array array{};

    array.fill(test_enum::third);

    int sum = 0;

    for (const auto value : array)
        sum += value;

    EXPECT_EQ(sum, 9);
}

// Const container interface

TEST(enum_array, const_iterators) {
    test_array array{};

    array[test_enum::first] = 1;
    array[test_enum::second] = 2;
    array[test_enum::third] = 3;

    const test_enum& constArray = array;

    EXPECT_EQ(std::distance(constArray.begin(), constArray.end()), 3);
}

// Initialization

TEST(enum_array, aggregate_initialization) {
    test_array array{3, 2, 1};

    EXPECT_EQ(array[test_enum::first], 3);
    EXPECT_EQ(array[test_enum::second], 2);
    EXPECT_EQ(array[test_enum::third], 1);
}
