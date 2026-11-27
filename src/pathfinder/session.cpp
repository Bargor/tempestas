#include "session.h"

#include <ecs/registry.h>
#include <util/dice_roller.h>

namespace tst::pathfinder {

session::session(ecs::registry& registry, dice_roller& roller)
    : m_registry(registry)
    , m_roller(roller)
    , m_game_state(m_registry)
    , m_encounter_system(m_game_state, m_roller, m_registry.get_storage<perception>()) {
}
} // namespace tst::pathfinder
