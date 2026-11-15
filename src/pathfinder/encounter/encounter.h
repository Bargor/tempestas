#pragma once

#include "ecs/registry.h"
#include "encounter/turn_system.h"
#include "session.h"

#include <vector>

namespace tst::pathfinder {

class encounter_system {
public:
    encounter_system(ecs::registry& registry, dice_roller& roller);

    void start_encounter(std::vector<ecs::entity>& entities);
    void finish_encounter();

private:
    ecs::registry& m_registry;
    dice_roller& m_roller;

    turn_system m_turn_system;

    bool m_encounter_active{false};
};

} // namespace tst::pathfinder