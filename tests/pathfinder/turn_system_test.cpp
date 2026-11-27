#include "pathfinder/encounter/turn_system.h"
#include "pathfinder/util/dice_roller.h"

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
    for (uint32_t i = 0; i < 10; ++i) {
        auto entity = ecs::entity{.id = i, .generation = 0};
        entities.push_back(entity);
        perception_set.emplace(entity,
                               perception{.perception = static_cast<int32_t>(i),
                                          .rank = proficency_rank::untrained,
                                          .wisdom = 10,
                                          .item_bonus = 0});
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
    for (uint32_t i = 0; i < 10; ++i) {
        auto entity = ecs::entity{.id = i, .generation = 0};
        entities.push_back(entity);
        perception_set.emplace(entity,
                               perception{.perception = static_cast<int32_t>(i),
                                          .rank = proficency_rank::untrained,
                                          .wisdom = 10,
                                          .item_bonus = 0});
    }
    system.roll_initiative(entities);
    const auto& order = system.get_initiative_order();

    for (size_t i = 0; i < 10; ++i) {
        const auto entity = system.next_turn();
        EXPECT_NE(entity, ecs::invalid_entity);
        EXPECT_EQ(entity, order[i].entity);
    }
}

TEST(turn_system, next_turn_wrap_around) {
    ecs::sparse_set<perception> perception_set;
    dice_roller roller(42);
    turn_system system(perception_set, roller);
    std::vector<ecs::entity> entities;
    for (uint32_t i = 0; i < 10; ++i) {
        auto entity = ecs::entity{.id = i, .generation = 0};
        entities.push_back(entity);
        perception_set.emplace(entity,
                               perception{.perception = static_cast<int32_t>(i),
                                          .rank = proficency_rank::untrained,
                                          .wisdom = 10,
                                          .item_bonus = 0});
    }
    system.roll_initiative(entities);
    const auto& order = system.get_initiative_order();
    for (size_t i = 0; i < 10; ++i) {
        const auto entity = system.next_turn();
        EXPECT_NE(entity, ecs::invalid_entity);
        EXPECT_EQ(entity, order[i].entity);
    }
    // Next turn should wrap around to the first entity
    const auto entity = system.next_turn();
    EXPECT_NE(entity, ecs::invalid_entity);
    EXPECT_EQ(entity, order[0].entity);
}

TEST(turn_system, get_initiative_order_without_roll) {
    ecs::sparse_set<perception> perception_set;
    dice_roller roller(42);
    turn_system system(perception_set, roller);
    EXPECT_DEATH(system.get_initiative_order(), "");
}

TEST(turn_system, get_current_phase_without_roll) {
    ecs::sparse_set<perception> perception_set;
    dice_roller roller(42);
    turn_system system(perception_set, roller);
    EXPECT_DEATH(system.get_current_phase(), "");
}

TEST(turn_system, test_finish_encounter) {
    ecs::sparse_set<perception> perception_set;
    dice_roller roller(42);
    turn_system system(perception_set, roller);
    EXPECT_DEATH(system.finish_encounter(), "Encounter is not active");
}

TEST(turn_system, test_finish_encounter_after_roll) {
    ecs::sparse_set<perception> perception_set;
    dice_roller roller(42);
    turn_system system(perception_set, roller);
    std::vector<ecs::entity> entities;
    for (uint32_t i = 0; i < 10; ++i) {
        auto entity = ecs::entity{.id = i, .generation = 0};
        entities.push_back(entity);
        perception_set.emplace(entity,
                               perception{.perception = static_cast<int32_t>(i),
                                          .rank = proficency_rank::untrained,
                                          .wisdom = 10,
                                          .item_bonus = 0});
    }
    system.roll_initiative(entities);
    system.finish_encounter();
    EXPECT_DEATH(system.get_initiative_order(), "");
    EXPECT_DEATH(system.get_current_phase(), "");
}

TEST(turn_system, test_roll_initiative_after_finish_encounter) {
    ecs::sparse_set<perception> perception_set;
    dice_roller roller(42);
    turn_system system(perception_set, roller);
    std::vector<ecs::entity> entities;
    for (uint32_t i = 0; i < 10; ++i) {
        auto entity = ecs::entity{.id = i, .generation = 0};
        entities.push_back(entity);
        perception_set.emplace(entity,
                               perception{.perception = static_cast<int32_t>(i),
                                          .rank = proficency_rank::untrained,
                                          .wisdom = 10,
                                          .item_bonus = 0});
    }
    system.roll_initiative(entities);
    system.finish_encounter();
    system.roll_initiative(entities);
    const auto& order = system.get_initiative_order();
    EXPECT_EQ(order.size(), entities.size());
}
