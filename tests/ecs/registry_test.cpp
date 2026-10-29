#include "ecs/registry.h"

#include <gtest/gtest.h>

using namespace tst::ecs;

struct test_component {
    int int_value;
    float float_value;

    bool operator==(const test_component&) const = default;
};

struct another_component {
    double double_value;
    char char_value;
    bool operator==(const another_component&) const = default;
};

TEST(registry, create_entity) {
    registry reg;
    entity e = reg.create_entity();
    EXPECT_TRUE(reg.is_valid(e));
}

TEST(registry, create_entity_and_destroy) {
    registry reg;
    entity e = reg.create_entity();
    EXPECT_TRUE(reg.is_valid(e));
    EXPECT_TRUE(reg.destroy_entity(e));
    EXPECT_FALSE(reg.is_valid(e));
}

TEST(registry, create_storage) {
    registry reg;
    const auto& storage = reg.register_component<test_component>();
    EXPECT_EQ(storage.size(), 0);
}

TEST(registry, entity_is_valid) {
    registry reg;
    entity e = reg.create_entity();
    EXPECT_TRUE(reg.is_valid(e));
}

TEST(registry, entity_not_valid_after_destroy) {
    registry reg;
    entity e = reg.create_entity();
    EXPECT_TRUE(reg.is_valid(e));
    EXPECT_TRUE(reg.destroy_entity(e));
    EXPECT_FALSE(reg.is_valid(e));
}

TEST(registry, entity_not_valid) {
    registry reg;
    entity e{ .id = 1, .generation = 0};
    EXPECT_FALSE(reg.is_valid(e));
}

TEST(registry, destroy_entity_false) {
    registry reg;
    entity e{.id = 1, .generation = 0};
    EXPECT_FALSE(reg.destroy_entity(e));
}

TEST(registry, create_component_and_add) {
    registry reg;
    entity e = reg.create_entity();
    auto& storage = reg.register_component<test_component>();
    storage.emplace(e, test_component{42, 3.14f});
    EXPECT_EQ(storage.size(), 1);
    EXPECT_TRUE(reg.contains<test_component>(e));
}

TEST(registry, create_component_and_get) {
    registry reg;
    entity e = reg.create_entity();
    auto& storage = reg.register_component<test_component>();
    storage.emplace(e, test_component{42, 3.14f});
    EXPECT_EQ(storage.size(), 1);
    EXPECT_TRUE(reg.contains<test_component>(e));
    auto* component = reg.get<test_component>(e);
    ASSERT_NE(component, nullptr);
    EXPECT_EQ(component->int_value, 42);
    EXPECT_EQ(component->float_value, 3.14f);
}

TEST(registry, create_and_erase_component) {
    registry reg;
    entity e = reg.create_entity();
    auto& storage = reg.register_component<test_component>();
    storage.emplace(e, test_component{42, 3.14f});
    EXPECT_EQ(storage.size(), 1);
    EXPECT_TRUE(reg.contains<test_component>(e));
    EXPECT_TRUE(reg.erase<test_component>(e));
    EXPECT_EQ(storage.size(), 0);
    EXPECT_FALSE(reg.contains<test_component>(e));
}

TEST(registry, reuse_entity) {
    registry reg;
    entity e = reg.create_entity();
    const auto entity_id = e.id;
    const auto generation = e.generation;
    EXPECT_TRUE(reg.destroy_entity(e));
    entity e2 = reg.create_entity();
    EXPECT_EQ(e2.id, entity_id);
    EXPECT_GT(e2.generation, generation);
}
