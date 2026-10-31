#include <registry.h>

namespace tst::ecs {

entity registry::create_entity() {
    uint32_t index = [&]() {
        // Implementation for selecting an index
        if (m_free_indices.empty()) {
            m_slots.emplace_back(0, true);
            return static_cast<uint32_t>(m_slots.size() - 1);
        } else {
            uint32_t index = m_free_indices.back();
            m_free_indices.pop_back();
            m_slots[index].generation++;
            m_slots[index].occupied = true;
            return index;
        }
    }();

    return entity{index, m_slots[index].generation};
}

bool registry::destroy_entity(entity id) {
    if (!is_valid(id)) {
        // Invalid entity, handle error as needed
        return false;
    }

    for (auto& [componentId, storage] : m_component_storages) {
        storage->erase(id);
    }

    m_slots[id.id].occupied = false;
    m_free_indices.push_back(id.id);
    return true;
}

bool registry::is_valid(entity e) const {
    if (e.id >= m_slots.size()) [[unlikely]] {
        return false;
    }
    const auto& slot = m_slots[e.id];
    return slot.occupied && slot.generation == e.generation;
}

} // namespace tst::ecs