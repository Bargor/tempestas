#pragma once

#include "platform.h"

#include <cstdint>
#include <random>
#include <vector>

namespace tst::pathfinder {

struct dice_group {
    int32_t count{};
    int32_t sides{};
    int32_t modifier{};
};

class dice_roll {
    friend class dice_roller;

public:
    dice_roll() = default;
    dice_roll(dice_group& group);

    dice_roll& add(const dice_group& group);
    dice_roll& add_modifier(int32_t modifier);

private:
    std::vector<dice_group> m_groups;
    int32_t global_modifier{};
};

class dice_roller {
public:
    dice_roller(uint32_t seed);

    int32_t roll(const dice_roll& roll);

private:
    std::mt19937 m_generator;
};

TST_INLINE dice_roll& dice_roll::add_modifier(int32_t modifier) {
    this->global_modifier += modifier;
    return *this;
}

} // namespace tst::pathfinder