#include "ecs/sparse_set.h"

#include <gtest/gtest.h>

using namespace tst::ecs;

struct test_component {
    int int_value;
    float float_value;

    bool operator==(const test_component&) const = default;
};

TEST(sparse_set, size_changes_after_add) {
    sparse_set<test_component> set{};

    EXPECT_EQ(set.size(), 0);

    set.emplace(0, 42, 3.14f);
    EXPECT_EQ(set.size(), 1);

    set.emplace(1, test_component{24, 2.71f});
    EXPECT_EQ(set.size(),    2);
}

TEST(sparse_set, emplace) {
    sparse_set<test_component> set{};

    set.emplace(0, 42, 3.14f);
    EXPECT_EQ(set.size(), 1);

    set.emplace(1, test_component{24, 2.71f});
    EXPECT_EQ(set.size(), 2);

    auto& component_0 = *set.get(0);
    EXPECT_EQ(component_0.int_value, 42);
    EXPECT_EQ(component_0.float_value, 3.14f);

    auto& component_1 = *set.get(1);
    EXPECT_EQ(component_1.int_value, 24);
    EXPECT_EQ(component_1.float_value, 2.71f);
}

TEST(sparse_set, emplace_twice_same_id) {
    sparse_set<test_component> set{};

    set.emplace(0, 42, 3.14f);
    EXPECT_EQ(set.size(), 1);

    auto& component = set.emplace(0, test_component{24, 2.71f});
    EXPECT_EQ(component.int_value, 42);
    EXPECT_EQ(component.float_value, 3.14f);
    EXPECT_EQ(set.size(), 1);

    auto& component_0 = *set.get(0);
    EXPECT_EQ(component_0.int_value, 42);
    EXPECT_EQ(component_0.float_value, 3.14f);
}

TEST(sparse_set, contains_true) {
    sparse_set<test_component> set{};

    EXPECT_EQ(set.size(), 0);

    set.emplace(0, 42, 3.14f);
    EXPECT_EQ(set.size(), 1);

    EXPECT_TRUE(set.contains(0));
}

TEST(sparse_set, contains_false) {
    sparse_set<test_component> set{};

    EXPECT_EQ(set.size(), 0);

    set.emplace(0, 42, 3.14f);
    EXPECT_EQ(set.size(), 1);

    EXPECT_FALSE(set.contains(1));
}

TEST(sparse_set, get) {
    sparse_set<test_component> set{};

    EXPECT_EQ(set.size(), 0);

    set.emplace(0, 42, 3.14f);
    EXPECT_EQ(set.size(), 1);
    
    auto& component = *set.get(0);
    EXPECT_EQ(component.int_value, 42);
    EXPECT_EQ(component.float_value, 3.14f);

    const auto& const_component = *set.get(0);
    EXPECT_EQ(const_component.int_value, 42);
    EXPECT_EQ(const_component.float_value, 3.14f);
}

TEST(sparse_set, erase_size_decreases) {
    sparse_set<test_component> set{};

    EXPECT_EQ(set.size(), 0);

    set.emplace(0, 42, 3.14f);
    EXPECT_EQ(set.size(), 1);

    bool erased = set.erase(0);
    EXPECT_TRUE(erased);
    EXPECT_EQ(set.size(), 0);
}

TEST(sparse_set, erase_false) {
    sparse_set<test_component> set{};

    EXPECT_EQ(set.size(), 0);

    set.emplace(0, 42, 3.14f);
    EXPECT_EQ(set.size(), 1);

    bool erased = set.erase(1);
    EXPECT_FALSE(erased);
    EXPECT_EQ(set.size(), 1);
}

TEST(sparse_set, erase_get_returns_nullptr) {
    sparse_set<test_component> set{};

    EXPECT_EQ(set.size(), 0);

    set.emplace(0, 42, 3.14f);
    EXPECT_EQ(set.size(), 1);

    bool erased = set.erase(0);
    EXPECT_TRUE(erased);
    EXPECT_EQ(set.size(), 0);

    auto component = set.get(0);
    EXPECT_EQ(component, nullptr);
}

TEST(sparse_set, erase_get_another_component) {
    sparse_set<test_component> set{};

    set.emplace(0, 42, 3.14f);
    set.emplace(1, test_component{24, 2.71f});
    EXPECT_EQ(set.size(), 2);

    bool erased = set.erase(0);
    EXPECT_TRUE(erased);
    EXPECT_EQ(set.size(), 1);

    auto& component = *set.get(1);
    test_component gold{24, 2.71f};
    EXPECT_EQ(component, gold);
}

TEST(sparse_set, erase_twice_same_id) {
    sparse_set<test_component> set{};

    set.emplace(0, 42, 3.14f);
    set.emplace(1, test_component{24, 2.71f});
    EXPECT_EQ(set.size(), 2);

    bool erased = set.erase(0);
    EXPECT_TRUE(erased);
    EXPECT_EQ(set.size(), 1);

    auto deleted_component = set.get(0);
    EXPECT_EQ(deleted_component, nullptr);

    auto& component = *set.get(1);
    test_component gold{24, 2.71f};
    EXPECT_EQ(component, gold);

    erased = set.erase(0);
    EXPECT_FALSE(erased);
    EXPECT_EQ(set.size(), 1);

    auto deleted_component_second = set.get(0);
    EXPECT_EQ(deleted_component_second, nullptr);
}

TEST(sparse_set, insert_erase_insert) {
    sparse_set<test_component> set{};

    set.emplace(0, 42, 3.14f);
    set.emplace(1, test_component{24, 2.71f});
    EXPECT_EQ(set.size(), 2);

    bool erased = set.erase(0);
    EXPECT_TRUE(erased);
    EXPECT_EQ(set.size(), 1);

    auto deleted_component = set.get(0);
    EXPECT_EQ(deleted_component, nullptr);

    set.emplace(0, 42, 3.14f);
    EXPECT_EQ(set.size(), 2);

    auto& inserted_component = *set.get(0);
    test_component gold{42, 3.14f};
    EXPECT_EQ(inserted_component, gold);
}
