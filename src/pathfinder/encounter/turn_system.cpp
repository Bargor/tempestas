#include "turn_system.h"

namespace tst::pathfinder {

turn_system::turn_system(dice_roller& roller) : m_roller(roller), m_current_index(0) {
}

void roll_initiative(std::vector<ecs::entity>& entities) {
    // TODO: Use only perception for rolling initiative, implement another skills later
    for (auto& entity : entities) {
        auto perception = entity.get_component<perception>();
        auto initiative_roll = m_roller.roll() + perception.value();
        m_initiative_order.emplace_back(entity, initiative_roll);
    }
}

} // namespace tst::pathfinder