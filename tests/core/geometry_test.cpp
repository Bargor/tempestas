// This file is part of Tempest-core project
// Author: Karol Kontny

#include <functional>
#include <gtest/gtest.h>
#include <position.h>
#include <rectangle.h>

using namespace tst::core;

TEST(position, equality_true_for_equal_values) {
    const position<int> lhs{10, 20};
    const position<int> rhs{10, 20};

    EXPECT_TRUE(lhs == rhs);
}

TEST(position, equality_false_when_any_field_differs) {
    const position<int> left_diff{9, 20};
    const position<int> right_diff{10, 21};
    const position<int> base{10, 20};

    EXPECT_FALSE(base == left_diff);
    EXPECT_FALSE(base == right_diff);
}

TEST(extent, equality_checks_both_dimensions) {
    const extent<int> base{100, 200};
    const extent<int> same{100, 200};
    const extent<int> different_width{101, 200};
    const extent<int> different_height{100, 201};

    EXPECT_TRUE(base == same);
    EXPECT_FALSE(base == different_width);
    EXPECT_FALSE(base == different_height);
}

TEST(rectangle, equality_requires_equal_offset_and_dimensions) {
    const rectangle<int, int> base{{10, 20}, {100, 200}};
    const rectangle<int, int> same{{10, 20}, {100, 200}};
    const rectangle<int, int> different_offset{{11, 20}, {100, 200}};
    const rectangle<int, int> different_dimensions{{10, 20}, {99, 200}};

    EXPECT_TRUE(base == same);
    EXPECT_FALSE(base == different_offset);
    EXPECT_FALSE(base == different_dimensions);
}

TEST(hash, equal_geometry_values_have_equal_hashes) {
    const position<int> p1{10, 20};
    const position<int> p2{10, 20};

    const extent<int> e1{100, 200};
    const extent<int> e2{100, 200};

    const rectangle<int, int> r1{p1, e1};
    const rectangle<int, int> r2{p2, e2};

    const size_t position_hash_1 = std::hash<position<int>>{}(p1);
    const size_t position_hash_2 = std::hash<position<int>>{}(p2);
    const size_t extent_hash_1 = std::hash<extent<int>>{}(e1);
    const size_t extent_hash_2 = std::hash<extent<int>>{}(e2);
    const size_t rectangle_hash_1 = std::hash<rectangle<int, int>>{}(r1);
    const size_t rectangle_hash_2 = std::hash<rectangle<int, int>>{}(r2);

    EXPECT_EQ(position_hash_1, position_hash_2);
    EXPECT_EQ(extent_hash_1, extent_hash_2);
    EXPECT_EQ(rectangle_hash_1, rectangle_hash_2);
}
