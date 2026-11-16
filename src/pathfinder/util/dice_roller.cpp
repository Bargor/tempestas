#include "dice_roller.h"

namespace tst::pathfinder {

dice_roll::dice_roll(dice_group& group) {
    m_groups.push_back(group);
}

dice_roll& dice_roll::add(const dice_group& group) {
    m_groups.push_back(group);
    return *this;
}

dice_roller::dice_roller(uint32_t seed) {
    m_generator.seed(seed);
}

int64_t dice_roller::roll(const dice_roll& roll) {
    int64_t result = 0;

    for (const auto& group : roll.m_groups) {
        std::uniform_int_distribution<int64_t> distribution(1, group.sides);
        for (int64_t i = 0; i < group.count; ++i) {
            result += distribution(m_generator);
        }
        result += group.modifier;
    }
    result += roll.global_modifier;

    return result;
}
} // namespace tst::pathfinder