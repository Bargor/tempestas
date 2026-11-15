#include "encounter.h"

#include "components/common.h"

namespace tst::pathfinder {

encounter_system::encounter_system(ecs::registry& registry, dice_roller& roller)
    : m_registry(registry), m_roller(roller), m_turn_system(registry.get_storage<perception>(), m_roller) {
}

void encounter_system::start_encounter(std::vector<ecs::entity>& entities) {
    hard_assert(!m_encounter_active, "Encounter is already active");
    m_turn_system.roll_initiative(entities);

    m_encounter_active = true;
}

void encounter_system::finish_encounter() {
    hard_assert(m_encounter_active, "Encounter is not active");
    m_encounter_active = false;
}

} // namespace tst::pathfinder