#pragma once

#include "assert.h"
#include "entity.h"

#include <fmt/printf.h>
#include <limits>
#include <type_traits>
#include <vector>

namespace tst::ecs {

template<typename Component>
concept SparseSetComponent = std::is_trivially_default_constructible_v<Component> &&
    std::is_trivially_copyable_v<Component> &&
    std::is_trivially_destructible_v<Component>;

template<SparseSetComponent Component>
class sparse_set {
public:
    sparse_set() = default;

    [[nodiscard]] bool contains(entity e) const noexcept;

    template<typename... Args>
    Component& emplace(entity e, Args&&... args);
    Component& emplace(entity e, Component&& component);
    Component* get(entity e) noexcept;
    const Component* get(entity e) const noexcept;
    bool erase(entity e);
    [[nodiscard]] size_t size() const noexcept {
        return components.size();
    }

private:
    static constexpr size_t npos = std::numeric_limits<size_t>::max();

    std::vector<Component> components;
    std::vector<entity> entities;

    std::vector<size_t> entity_id_to_index;
};

template<SparseSetComponent Component>
bool sparse_set<Component>::contains(entity e) const noexcept {
    if (e.id >= entity_id_to_index.size()) {
        return false;
    }
    size_t index = entity_id_to_index[e.id];
    return index != npos && index < components.size() && entities[index] == e;
}

template<SparseSetComponent Component>
template<typename... Args>
Component& sparse_set<Component>::emplace(entity e, Args&&... args) {
    if (contains(e)) {
        return components[entity_id_to_index[e.id]];
    }

    if (e.id >= entity_id_to_index.size()) {
        entity_id_to_index.resize(e.id + 1, npos);
    }
    assert(entity_id_to_index[e.id] == npos);

    const size_t index = components.size();

    // TODO: Use custom allocator to ensure it doesn't throw
    components.emplace_back(std::forward<Args>(args)...);
    entities.push_back(e);
    entity_id_to_index[e.id] = index;
    return components.back();
}

template<SparseSetComponent Component>
Component& sparse_set<Component>::emplace(entity e, Component&& component) {
    if (contains(e)) {
        return components[entity_id_to_index[e.id]];
    }

    if (e.id >= entity_id_to_index.size()) {
        entity_id_to_index.resize(e.id + 1, npos);
    }
    if (entity_id_to_index[e.id] != npos) [[unlikely]] {
        fmt::printf("Inserting component to ECS failed\n");
        std::abort();
    }

    const size_t index = components.size();

    // TODO: Use custom allocator to ensure it doesn't throw
    components.push_back(std::forward<Component>(component));
    entities.push_back(e);
    entity_id_to_index[e.id] = index;
    return components.back();
}

template<SparseSetComponent Component>
Component* sparse_set<Component>::get(entity e) noexcept {
    if (!contains(e)) {
        return nullptr;
    }
    size_t index = entity_id_to_index[e.id];
    return &components[index];
}

template<SparseSetComponent Component>
const Component* sparse_set<Component>::get(entity e) const noexcept {
    if (!contains(e)) {
        return nullptr;
    }
    size_t index = entity_id_to_index[e.id];
    return &components[index];
}

template<SparseSetComponent Component>
bool sparse_set<Component>::erase(entity e) {
    if (!contains(e)) {
        return false;
    }
    size_t index = entity_id_to_index[e.id];
    size_t last_index = components.size() - 1;

    if (index != last_index) {
        const auto moved = entities[last_index];
        // Update the index of the swapped entity
        entity_id_to_index[moved.id] = index;
        // Swap the component to be removed with the last component
        entities[index] = moved;
        components[index] = std::move(components[last_index]);
    }

    // Remove the last component and entity ID
    components.pop_back();
    entities.pop_back();
    entity_id_to_index[e.id] = npos;
    return true;
}

} // namespace tst::ecs