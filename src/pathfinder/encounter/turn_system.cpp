#include "turn_system.h"

#include <assert.h>

namespace tst::pathfinder {

turn_system::turn_system(ecs::sparse_set<perception>& perception_set, dice_roller& roller)
    : m_perception_set(perception_set), m_roller(roller), m_current_index(0) {
}

void turn_system::roll_initiative(std::vector<ecs::entity>& entities) {
    hard_assert(!m_encounter_active, "Encounter is already active");

    // TODO: Use only perception for rolling initiative, implement another skills later
    for (auto& entity : entities) {
        const auto perception = m_perception_set.get(entity);
        dice_roll roll;
        roll.add(dice_group{.count = 1, .sides = 20, .modifier = 0});
        roll.add_modifier(perception->perception);
        auto initiative_roll = m_roller.roll(roll);
        m_initiative_order.emplace_back(entity, initiative_roll);
    }

    std::sort(m_initiative_order.begin(),
              m_initiative_order.end(),
              [](const initiative_data& a, const initiative_data& b) { return a.initiative > b.initiative; });

    m_encounter_active = true;

    for (size_t i = 0; i < m_default_rounds; ++i) {
        size_t current_entity_idx = 0;
        for (int32_t j = 0; j < static_cast<int32_t>(round_phase::enum_size); ++j) {
            auto& data = m_phases.emplace_back(phase_data{.phase = static_cast<round_phase>(j), .round = i + 1});
            switch (data.phase) {
                case round_phase::turn_start:
                case round_phase::regain_resources:
                case round_phase::main_turn:
                case round_phase::turn_end: {
                    data.entity = m_initiative_order[current_entity_idx].entity;
                    break;
                }
            }
        }
    }
}

const turn_system::phase_data& turn_system::get_current_phase() const {
    return m_phases[m_current_index];
}

} // namespace tst::pathfinder