#include <registry.h>

namespace tst::ecs {

entity registry::create_entity() {
    uint32_t index = [&]() {
        // Implementation for selecting an index
        if (free_indices.empty()) {
            slots.emplace_back(0, true);
            return static_cast<uint32_t>(slots.size() - 1);
        } else {
            uint32_t index = free_indices.back();
            free_indices.pop_back();
            slots[index].generation++;
            return index;
        }
    }();

    return entity{index, slots[index].generation};
}

bool registry::destroy_entity(entity id) {
    if (!is_valid(id)) {
        // Invalid entity, handle error as needed
        return false;
    }

    for (auto& [componentId, storage] : component_storages) {
        storage->erase(id);
    }

    slots[id.id].occupied = false;
    free_indices.push_back(id.id);
    return true;
}

bool registry::is_valid(entity e) const {
    if (e.id >= slots.size()) [[unlikely]] {
        return false;
    }
    const auto& slot = slots[e.id];
    return slot.occupied && slot.generation == e.generation;
}

} // namespace tst::ecs