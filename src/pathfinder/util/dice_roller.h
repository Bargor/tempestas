#pragma once

#include "platform.h"

#include <cstdint>
#include <random>
#include <vector>

namespace tst::pathfinder {

struct dice_group {
    int64_t count{};
    int64_t sides{};
    int64_t modifier{};
};

class dice_roll {
    friend class dice_roller;

public:
    dice_roll() = default;
    dice_roll(dice_group& group);

    dice_roll& add(const dice_group& group);
    dice_roll& add_modifier(int64_t modifier);

private:
    std::vector<dice_group> m_groups;
    int64_t global_modifier{};
};

class dice_roller {
public:
    dice_roller(uint32_t seed);

    int64_t roll(const dice_roll& roll);

private:
    std::mt19937 m_generator;
};

TST_INLINE dice_roll& dice_roll::add_modifier(int64_t modifier) {
    this->global_modifier += modifier;
    return *this;
}

} // namespace tst::pathfinder