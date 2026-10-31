#pragma once

#include "core.h"
#include "entity.h"
#include "sparse_set.h"

#include <core/util/type_name.h>
#include <fmt/printf.h>
#include <unordered_map>
#include <vector>

namespace tst::ecs {

class registry {
    struct slot {
        uint32_t generation = 0;
        bool occupied = false;
    };

    struct storage_base {
        virtual ~storage_base() = default;
        virtual bool erase(entity e) = 0;
    };

    template<SparseSetComponent Component>
    struct sparse_set_storage : storage_base {
        sparse_set<Component> set;
        bool erase(entity e) override {
            return set.erase(e);
        }
    };

public:
    registry() = default;
    registry(const registry&) = delete;
    registry& operator=(const registry&) = delete;

    registry(registry&&) = default;
    registry& operator=(registry&&) = default;

    [[nodiscard]] entity create_entity();
    bool destroy_entity(entity id);

    template<SparseSetComponent Component>
    sparse_set<Component>& register_component();

    template<SparseSetComponent Component>
    sparse_set<Component>& get_storage();

    template<SparseSetComponent Component, typename... Args>
    Component& emplace(entity e, Args&&... args);

    template<SparseSetComponent Component>
    Component* get(entity e) noexcept;

    template<SparseSetComponent Component>
    const Component* get(entity e) const noexcept;

    template<SparseSetComponent Component>
    bool contains(entity e) const noexcept;

    bool is_valid(entity e) const;

    template<SparseSetComponent Component>
    bool erase(entity e);

private:
    using ComponentId = size_t;

    static ComponentId next_component_id() noexcept {
        static ComponentId cur_component_id = 0;
        return cur_component_id++;
    }

    template<SparseSetComponent Component>
    static ComponentId get_component_id() {
        static const ComponentId id = next_component_id();
        return id;
    }

    template<SparseSetComponent Component>
    sparse_set_storage<Component>* find_storage() noexcept;

    template<SparseSetComponent Component>
    const sparse_set_storage<Component>* find_storage() const noexcept;

private:
    std::vector<slot> m_slots;
    std::vector<entity_id> m_free_indices;

    std::unordered_map<ComponentId, ptr<storage_base>> m_component_storages;
};

template<SparseSetComponent Component>
registry::sparse_set_storage<Component>* registry::find_storage() noexcept {
    const ComponentId component_id = get_component_id<Component>();
    auto it = m_component_storages.find(component_id);
    return it != m_component_storages.end() ? static_cast<sparse_set_storage<Component>*>(it->second.get()) : nullptr;
}

template<SparseSetComponent Component>
const registry::sparse_set_storage<Component>* registry::find_storage() const noexcept {
    const ComponentId component_id = get_component_id<Component>();
    const auto it = m_component_storages.find(component_id);
    return it != m_component_storages.end() ? static_cast<const sparse_set_storage<Component>*>(it->second.get()) :
                                              nullptr;
}

template<SparseSetComponent Component>
sparse_set<Component>& registry::register_component() {
    ComponentId component_id = get_component_id<Component>();
    auto find_it = m_component_storages.find(component_id);

    if (find_it != m_component_storages.end()) {
        fmt::printf("Component already registered: id=%s\n", core::type_name<Component>());
        std::abort();
    }
    auto it = m_component_storages.emplace(component_id, std::make_unique<sparse_set_storage<Component>>());
    if (!it.second) {
        std::abort();
    }
    return static_cast<sparse_set_storage<Component>*>(it.first->second.get())->set;
}

template<SparseSetComponent Component>
sparse_set<Component>& registry::get_storage() {
    ComponentId component_id = get_component_id<Component>();

    auto it = m_component_storages.find(component_id);

    if (it == m_component_storages.end()) {
        fmt::printf("Attempted to get storage for unregistered component: id=%s\n", core::type_name<Component>());
        std::abort();
    }
    return static_cast<sparse_set_storage<Component>*>(it->second.get())->set;
}

template<SparseSetComponent Component, typename... Args>
Component& registry::emplace(entity e, Args&&... args) {
    if (!is_valid(e)) {
        fmt::printf("Attempted to emplace component for invalid entity: id=%u, generation=%u\n",
                    static_cast<std::uint32_t>(e.id),
                    e.generation);
        std::abort();
    }
    return get_storage<Component>().emplace(e, std::forward<Args>(args)...);
}

template<SparseSetComponent Component>
Component* registry::get(entity e) noexcept {
    if (!is_valid(e)) {
        return nullptr;
    }
    auto* storage = find_storage<Component>();
    return storage ? storage->set.get(e) : nullptr;
}

template<SparseSetComponent Component>
const Component* registry::get(entity e) const noexcept {
    if (!is_valid(e)) {
        return nullptr;
    }
    const auto* storage = find_storage<Component>();
    return storage ? storage->set.get(e) : nullptr;
}

template<SparseSetComponent Component>
bool registry::contains(entity e) const noexcept {
    if (!is_valid(e)) {
        return false;
    }

    const auto* storage = find_storage<Component>();
    return storage ? storage->set.contains(e) : false;
}

template<SparseSetComponent Component>
bool registry::erase(entity e) {
    if (!is_valid(e)) {
        return false;
    }

    auto* storage = find_storage<Component>();
    return storage ? storage->erase(e) : false;
}

} // namespace tst::ecs