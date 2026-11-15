#include "pathfinder.h"

#include <ecs/registry.h>

namespace tst::pathfinder {

pathfinder::pathfinder(ecs::registry& registry, uint32_t seed)
    : m_registry(registry), m_roller(seed), m_encounter_system(m_registry, m_roller) {
}

} // namespace tst::pathfinder