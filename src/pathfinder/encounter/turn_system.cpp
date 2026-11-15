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
        for (size_t j = 0; j < m_initiative_order.size(); ++j) {
            for (int32_t k = 0; k < static_cast<int32_t>(round_phase::enum_size); ++k) {
                auto& data = m_phases.emplace_back(phase_data{
                    .entity = ecs::invalid_entity, .phase = static_cast<round_phase>(k), .round = i + 1, .events = {}});
                switch (data.phase) {
                    case round_phase::turn_start:
                    case round_phase::regain_resources:
                    case round_phase::main_turn:
                    case round_phase::turn_end: {
                        data.entity = m_initiative_order[j].entity;
                        break;
                    }
                    case round_phase::round_start:
                    case round_phase::round_end:
                    default: {
                        break;
                    }
                }
            }
        }
    }

    hard_assert(m_phases.size() == entities.size() * m_default_rounds * static_cast<size_t>(round_phase::enum_size),
                "Phases size is not equal to entities size * default rounds * round phase enum size");
}

ecs::entity turn_system::next_turn() {
    hard_assert(m_current_index < m_phases.size(), "Current index is out of bounds");

    m_current_index = m_current_index == 0 ? 0 : m_current_index + 1;

    for (; m_current_index < m_phases.size() && m_phases[m_current_index].phase != round_phase::main_turn;
         ++m_current_index) {
        // Skip phases until the next main turn
        if (m_phases[m_current_index].phase == round_phase::regain_resources) {
            // TODO: Perhaps we need kind of a resource manager thing
        }
    }
    hard_assert(m_current_index < m_phases.size(), "Current index is out of bounds");

    if (m_phases[m_current_index].phase == round_phase::round_start) {
        m_round++;
        m_current_index = 0;
    }

    return m_phases[m_current_index].entity;
}

const std::vector<turn_system::initiative_data>& turn_system::get_initiative_order() const noexcept {
    hard_assert(m_encounter_active, "Encounter is not active");
    return m_initiative_order;
}

const turn_system::phase_data& turn_system::get_current_phase() const {
    hard_assert(m_encounter_active, "Encounter is not active");
    return m_phases[m_current_index];
}

void turn_system::finish_encounter() noexcept {
    hard_assert(m_encounter_active, "Encounter is not active");
    m_encounter_active = false;
    m_current_index = 0;
    m_round = 0;
    m_initiative_order.clear();
    m_phases.clear();
}

} // namespace tst::pathfinder
