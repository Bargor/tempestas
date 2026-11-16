#include "pathfinder/util/dice_roller.h"

#include <gtest/gtest.h>

using namespace tst::pathfinder;

TEST(dice_roller, roll_dice) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 1, .sides = 6, .modifier = 0});
    int64_t result = roller.roll(roll);
    EXPECT_GE(result, 1);
    EXPECT_LE(result, 6);
}

TEST(dice_roller, roll_dice_multiple_times) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 1, .sides = 6, .modifier = 0});
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 1);
        EXPECT_LE(result, 6);
    }
}

TEST(dice_roller, roll_two_dice) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 2, .sides = 6, .modifier = 0});
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 2);
        EXPECT_LE(result, 12);
    }
}

TEST(dice_roller, roll_two_dice_with_global_modifier) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 2, .sides = 6, .modifier = 0});
    roll.add_modifier(5);
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 7);
        EXPECT_LE(result, 17);
    }
}

TEST(dice_roller, roll_dice_with_modifier) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 1, .sides = 6, .modifier = 2});
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 3);
        EXPECT_LE(result, 8);
    }
}

TEST(dice_roller, roll_multiple_groups) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 1, .sides = 6, .modifier = 0});
    roll.add(dice_group{.count = 1, .sides = 8, .modifier = 0});
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 2);
        EXPECT_LE(result, 14);
    }
}

TEST(dice_roller, roll_multiple_groups_with_global_modifier) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 1, .sides = 6, .modifier = 0});
    roll.add(dice_group{.count = 1, .sides = 8, .modifier = 0});
    roll.add_modifier(5);
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 7);
        EXPECT_LE(result, 19);
    }
}

TEST(dice_roller, roll_multiple_groups_with_modifier) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 1, .sides = 6, .modifier = 0});
    roll.add(dice_group{.count = 1, .sides = 8, .modifier = 2});
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 4);
        EXPECT_LE(result, 16);
    }
}

TEST(dice_roller, roll_multiple_groups_with_modifier_and_global_modifier) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 1, .sides = 6, .modifier = 0});
    roll.add(dice_group{.count = 1, .sides = 8, .modifier = 2});
    roll.add_modifier(5);
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 9);
        EXPECT_LE(result, 21);
    }
}

TEST(dice_roller, roll_multiple_groups_with_modifier_and_multiple_dice) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 2, .sides = 6, .modifier = 0});
    roll.add(dice_group{.count = 3, .sides = 8, .modifier = 2});
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 7);
        EXPECT_LE(result, 38);
    }
}

TEST(dice_roller, roll_multiple_groups_with_modifier_and_multiple_dice_and_negative_modifier) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 2, .sides = 6, .modifier = -2});
    roll.add(dice_group{.count = 3, .sides = 8, .modifier = -3});
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 2);
        EXPECT_LE(result, 33);
    }
}

TEST(dice_roller, roll_multiple_groups_with_modifier_and_multiple_dice_with_negative_modifier_and_global_modifier) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 2, .sides = 6, .modifier = -2});
    roll.add(dice_group{.count = 3, .sides = 8, .modifier = -3});
    roll.add_modifier(5);
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 7);
        EXPECT_LE(result, 38);
    }
}

TEST(dice_roller, roll_multiple_groups_with_modifier_and_multiple_dice_with_negative_modifier_and_global_zero_modifier) {
    dice_roller roller(42);
    dice_roll roll;
    roll.add(dice_group{.count = 2, .sides = 6, .modifier = -2});
    roll.add(dice_group{.count = 3, .sides = 8, .modifier = -3});
    roll.add(dice_group{.count = 1, .sides = 4, .modifier = 0});
    roll.add_modifier(0);
    for (int i = 0; i < 100; ++i) {
        int64_t result = roller.roll(roll);
        EXPECT_GE(result, 3);
        EXPECT_LE(result, 37);
    }
}