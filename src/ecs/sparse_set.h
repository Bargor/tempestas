#pragma once

#include "entity.h"

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

    bool contains(entity_id id) const;

    template<typename... Args>
    Component& emplace(entity_id id, Args&&... args);
    Component& emplace(entity_id id, Component&& component);
    Component* get(entity_id id);
    const Component* get(entity_id id) const;
    bool erase(entity_id id);
    size_t size() const {
        return components.size();
    }

private:
    static constexpr size_t npos = std::numeric_limits<size_t>::max();

    std::vector<Component> components;
    std::vector<entity_id> entity_ids;

    std::vector<size_t> entity_id_to_index;
};

template<SparseSetComponent Component>
bool sparse_set<Component>::contains(entity_id id) const {
    if (id >= entity_id_to_index.size()) {
        return false;
    }
    size_t index = entity_id_to_index[id];
    return index != npos && index < components.size() && entity_ids[index] == id;
}

template<SparseSetComponent Component>
template<typename... Args>
Component& sparse_set<Component>::emplace(entity_id id, Args&&... args) {
    if (contains(id)) {
        return components[entity_id_to_index[id]];
    }

    if (id >= entity_id_to_index.size()) {
        entity_id_to_index.resize(id + 1, npos);
    }
    size_t index = components.size();

    // TODO: Use custom allocator to ensure it doesn't throw
    components.emplace_back(std::forward<Args>(args)...);
    entity_ids.push_back(id);
    entity_id_to_index[id] = index;
    return components.back();
}

template<SparseSetComponent Component>
Component& sparse_set<Component>::emplace(entity_id id, Component&& component) {
    if (contains(id)) {
        return components[entity_id_to_index[id]];
    }

    if (id >= entity_id_to_index.size()) {
        entity_id_to_index.resize(id + 1, npos);
    }
    size_t index = components.size();

    // TODO: Use custom allocator to ensure it doesn't throw
    components.push_back(std::forward<Component>(component));
    entity_ids.push_back(id);
    entity_id_to_index[id] = index;
    return components.back();
}

template<SparseSetComponent Component>
Component* sparse_set<Component>::get(entity_id id) {
    if (!contains(id)) {
        return nullptr;
    }
    size_t index = entity_id_to_index[id];
    return &components[index];
}

template<SparseSetComponent Component>
const Component* sparse_set<Component>::get(entity_id id) const {
    if (!contains(id)) {
        return nullptr;
    }
    size_t index = entity_id_to_index[id];
    return &components[index];
}

template<SparseSetComponent Component>
bool sparse_set<Component>::erase(entity_id id) {
    if (!contains(id)) {
        return false;
    }
    size_t index = entity_id_to_index[id];
    size_t last_index = components.size() - 1;

    if (index != last_index) {
        const auto moved = entity_ids[last_index];
        // Update the index of the swapped entity
        entity_id_to_index[moved] = index;
        // Swap the component to be removed with the last component
        entity_ids[index] = moved;
        components[index] = std::move(components[last_index]);
    }

    // Remove the last component and entity ID
    components.pop_back();
    entity_ids.pop_back();
    entity_id_to_index[id] = npos;
    return true;
}

} // namespace tst::ecs