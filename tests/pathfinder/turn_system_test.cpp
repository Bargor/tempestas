#include "pathfinder/encounter/turn_system.h"

#include <gtest/gtest.h>

using namespace tst::pathfinder;
using namespace tst;

TEST(turn_system, init) {
    ecs::sparse_set<perception> perception_set;
    dice_roller roller(42);
    turn_system system(perception_set, roller);
}

TEST(turn_system, roll_initiative) {
    ecs::sparse_set<perception> perception_set;
    dice_roller roller(42);
    turn_system system(perception_set, roller);
    std::vector<ecs::entity> entities;
    for (int i = 0; i < 10; ++i) {
        auto entity = ecs::entity(i);
        entities.push_back(entity);
        perception_set.emplace(
            entity, perception{.perception = i, .rank = proficency_rank::untrained, .wisdom = 10, .item_bonus = 0});
    }
    system.roll_initiative(entities);
    const auto& phase_data = system.get_current_phase();
    EXPECT_EQ(phase_data.phase, round_phase::round_start);

    const auto& initiative_order = system.get_initiative_order();
    for (size_t i = 1; i < initiative_order.size(); ++i) {
        EXPECT_GE(initiative_order[i - 1].initiative, initiative_order[i].initiative);
    }
}

TEST(turn_system, next_turn) {
    ecs::sparse_set<perception> perception_set;
    dice_roller roller(42);
    turn_system system(perception_set, roller);
    std::vector<ecs::entity> entities;
    for (int i = 0; i < 10; ++i) {
        auto entity = ecs::entity(i);
        entities.push_back(entity);
        perception_set.emplace(
            entity, perception{.perception = i, .rank = proficency_rank::untrained, .wisdom = 10, .item_bonus = 0});
    }
    system.roll_initiative(entities);
    for (size_t i = 0; i < 100; ++i) {
        auto entity = system.next_turn();
        EXPECT_NE(entity, ecs::invalid_entity);
    }
}