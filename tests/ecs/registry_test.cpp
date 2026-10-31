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

TEST(registry, emplace_component) {
    registry reg;
    entity e = reg.create_entity();
    EXPECT_TRUE(reg.is_valid(e));
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    auto& component = reg.emplace<test_component>(e, test_component{42, 3.14f});
    EXPECT_EQ(component.int_value, 42);
    EXPECT_EQ(component.float_value, 3.14f);
    EXPECT_TRUE(reg.contains<test_component>(e));
}

TEST(registry, emplace_component_invalid_entity) {
    registry reg;
    entity e{.id = 1, .generation = 0};
    EXPECT_DEATH(reg.emplace<test_component>(e, test_component{42, 3.14f}), "");
}

TEST(registry, get_component_invalid_entity) {
    registry reg;
    entity e{.id = 1, .generation = 0};
    EXPECT_EQ(reg.get<test_component>(e), nullptr);
}

TEST(registry, contains_component_invalid_entity) {
    registry reg;
    entity e{.id = 1, .generation = 0};
    EXPECT_FALSE(reg.contains<test_component>(e));
}

TEST(registry, erase_component_invalid_entity) {
    registry reg;
    entity e{.id = 1, .generation = 0};
    EXPECT_FALSE(reg.erase<test_component>(e));
}

TEST(registry, get_component_not_registered) {
    registry reg;
    entity e = reg.create_entity();
    EXPECT_EQ(reg.get<test_component>(e), nullptr);
}

TEST(registry, contains_component_not_registered) {
    registry reg;
    entity e = reg.create_entity();
    EXPECT_FALSE(reg.contains<test_component>(e));
}

TEST(registry, erase_component_not_registered) {
    registry reg;
    entity e = reg.create_entity();
    EXPECT_FALSE(reg.erase<test_component>(e));
}

TEST(registry, get_component_const) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    const registry& const_reg = reg;
    const auto* component = const_reg.get<test_component>(e);
    ASSERT_NE(component, nullptr);
    EXPECT_EQ(component->int_value, 42);
    EXPECT_EQ(component->float_value, 3.14f);
}

TEST(registry, contains_component_const) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    const registry& const_reg = reg;
    EXPECT_TRUE(const_reg.contains<test_component>(e));
}

TEST(registry, multiple_components) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    [[maybe_unused]] auto& another_storage = reg.register_component<another_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    reg.emplace<another_component>(e, another_component{2.718, 'a'});
    EXPECT_TRUE(reg.contains<test_component>(e));
    EXPECT_TRUE(reg.contains<another_component>(e));
    auto* test_comp = reg.get<test_component>(e);
    auto* another_comp = reg.get<another_component>(e);
    ASSERT_NE(test_comp, nullptr);
    ASSERT_NE(another_comp, nullptr);
    EXPECT_EQ(test_comp->int_value, 42);
    EXPECT_EQ(test_comp->float_value, 3.14f);
    EXPECT_EQ(another_comp->double_value, 2.718);
    EXPECT_EQ(another_comp->char_value, 'a');
}

TEST(registry, erase_one_of_multiple_components) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    [[maybe_unused]] auto& another_storage = reg.register_component<another_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    reg.emplace<another_component>(e, another_component{2.718, 'a'});
    EXPECT_TRUE(reg.erase<test_component>(e));
    EXPECT_FALSE(reg.contains<test_component>(e));
    EXPECT_TRUE(reg.contains<another_component>(e));
}

TEST(registry, erase_all_components) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    [[maybe_unused]] auto& another_storage = reg.register_component<another_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    reg.emplace<another_component>(e, another_component{2.718, 'a'});
    EXPECT_TRUE(reg.erase<test_component>(e));
    EXPECT_TRUE(reg.erase<another_component>(e));
    EXPECT_FALSE(reg.contains<test_component>(e));
    EXPECT_FALSE(reg.contains<another_component>(e));
}

TEST(registry, destroy_entity_with_one_component) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    EXPECT_TRUE(reg.destroy_entity(e));
    EXPECT_FALSE(reg.is_valid(e));
    EXPECT_FALSE(reg.contains<test_component>(e));
}

TEST(registry, destroy_entity_with_components) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    [[maybe_unused]] auto& another_storage = reg.register_component<another_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    reg.emplace<another_component>(e, another_component{2.718, 'a'});
    EXPECT_TRUE(reg.destroy_entity(e));
    EXPECT_FALSE(reg.is_valid(e));
    EXPECT_FALSE(reg.contains<test_component>(e));
    EXPECT_FALSE(reg.contains<another_component>(e));
}

TEST(registry, reuse_entity_with_components) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    [[maybe_unused]] auto& another_storage = reg.register_component<another_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    reg.emplace<another_component>(e, another_component{2.718, 'a'});
    EXPECT_TRUE(reg.destroy_entity(e));
    entity e2 = reg.create_entity();
    EXPECT_EQ(e.id, e2.id);
    EXPECT_GT(e2.generation, e.generation);
    EXPECT_FALSE(reg.contains<test_component>(e2));
    EXPECT_FALSE(reg.contains<another_component>(e2));
}

TEST(registry, emplace_component_after_reuse_entity) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    EXPECT_TRUE(reg.destroy_entity(e));
    entity e2 = reg.create_entity();
    auto& component = reg.emplace<test_component>(e2, test_component{100, 6.28f});
    EXPECT_EQ(component.int_value, 100);
    EXPECT_EQ(component.float_value, 6.28f);
    EXPECT_TRUE(reg.contains<test_component>(e2));
}

TEST(registry, get_component_after_reuse_entity) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    EXPECT_TRUE(reg.destroy_entity(e));
    entity e2 = reg.create_entity();
    reg.emplace<test_component>(e2, test_component{100, 6.28f});
    auto* component = reg.get<test_component>(e2);
    ASSERT_NE(component, nullptr);
    EXPECT_EQ(component->int_value, 100);
    EXPECT_EQ(component->float_value, 6.28f);
}

TEST(registry, contains_component_after_reuse_entity) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    EXPECT_TRUE(reg.destroy_entity(e));
    entity e2 = reg.create_entity();
    reg.emplace<test_component>(e2, test_component{100, 6.28f});
    EXPECT_TRUE(reg.contains<test_component>(e2));
}

TEST(registry, erase_component_after_reuse_entity) {
    registry reg;
    entity e = reg.create_entity();
    [[maybe_unused]] auto& storage = reg.register_component<test_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    EXPECT_TRUE(reg.destroy_entity(e));
    entity e2 = reg.create_entity();
    reg.emplace<test_component>(e2, test_component{100, 6.28f});
    EXPECT_TRUE(reg.erase<test_component>(e2));
    EXPECT_FALSE(reg.contains<test_component>(e2));
}

TEST(registry, get_unregisted_storage) {
    registry reg;
    entity e = reg.create_entity();
    EXPECT_DEATH(reg.get_storage<test_component>(), "");
}

TEST(registry, get_registed_storage) {
    registry reg;
    entity e = reg.create_entity();
    reg.register_component<test_component>();
    auto& storage = reg.get_storage<test_component>();
    EXPECT_EQ(storage.size(), 0);
}

TEST(registry, get_storage_after_emplace) {
    registry reg;
    entity e = reg.create_entity();
    reg.register_component<test_component>();
    reg.emplace<test_component>(e, test_component{42, 3.14f});
    auto& storage = reg.get_storage<test_component>();
    EXPECT_EQ(storage.size(), 1);
}

TEST(registry, register_same_component_twice) {
    registry reg;
    entity e = reg.create_entity();
    reg.register_component<test_component>();
    EXPECT_DEATH(reg.register_component<test_component>(), "");
}
