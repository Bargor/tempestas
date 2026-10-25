#pragma once

#include "entity.h"
#include "sparse_set.h"

#include <vector>

namespace tst::ecs {

template<typename Component>
class sparse_set {
public:
    sparse_set() = default;

    bool contains(entity_id id) const;

    template<typename... Args>
    Component& emplace(entity_id id, Args&&... args);
    Component* get(entity_id id);
    const Component* get(entity_id id);
    bool erase(entity_id id);
    size_t size() const {
        return components.size();
    }

private:
    static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

    std::vector<Component> components;
    std::vector<entity_id> entity_ids;

    std::vector<std::size_t> entity_id_to_index;
};

template<typename Component>
bool sparse_set<Component>::contains(entity_id id) const {
    if (id >= entity_id_to_index.size()) {
        return false;
    }
    std::size_t index = entity_id_to_index[id];
    return index != npos && index < components.size() && entity_ids[index] == id;
}

template<typename Component>
template<typename... Args>
Component& sparse_set<Component>::emplace(entity_id id, Args&&... args) {
    if (contains(id)) {
        return components[entity_id_to_index[id]];
    }

    if (id >= entity_id_to_index.size()) {
        entity_id_to_index.resize(id + 1, npos);
    }
    std::size_t index = components.size();

    // TODO: Use custom allocator to ensure it doens't throw
    components.emplace_back(std::forward<Args>(args)...);
    entity_ids.push_back(id);
    entity_id_to_index[id] = index;
    return components.back();
}

template<typename Component>
Component* sparse_set<Component>::get(entity_id id) {
    if (!contains(id)) {
        return nullptr;
    }
    std::size_t index = entity_id_to_index[id];
    return &components[index];
}

template<typename Component>
const Component* sparse_set<Component>::get(entity_id id) {
    if (!contains(id)) {
        return nullptr;
    }
    std::size_t index = entity_id_to_index[id];
    return &components[index];
}

template<typename Component>
bool sparse_set<Component>::erase(entity_id id) {
    if (!contains(id)) {
        return false;
    }
    std::size_t index = entity_id_to_index[id];
    std::size_t last_index = components.size() - 1;

    if (index != last_index) {
        const auto moved = entity_ids[last_index];
        // Update the index of the swapped entity
        entity_id_to_index[moved] = index;
        // Swap the component to be removed with the last component
        entity_idx[index] = moved;
        components[index] = std::move(components[last_index]);
    }

    // Remove the last component and entity ID
    components.pop_back();
    entity_ids.pop_back();
    entity_id_to_index[id] = npos;
    return true;
}

class registry {
public:
    registry() = default;
    entity_id create_entity();
    void destroy_entity(entity_id id);
};

} // namespace tst::ecs