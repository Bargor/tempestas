#pragma once

#include "encounter/turn_system.h"
#include "game_state.h"

namespace tst::pathfinder {

class dice_roller;

class encounter_system {
public:
    encounter_system(game_state& state, dice_roller& roller, ecs::sparse_set<perception>& perception_set);

    void start_encounter(std::vector<ecs::entity>& entities);
    void finish_encounter();

private:
    game_state& m_state;
    dice_roller& m_roller;

    turn_system m_turn_system;

    bool m_encounter_active{false};
};

} // namespace tst::pathfinder