#pragma once

#include "ecs/entity.h"

#include <vector>

namespace tst::pathfinder {

class encounter_system {
    enum class encounter_state { before_start, rolling_initiative, in_progress, completed };

public:
    void start_encounter(std::vector<ecs::entity>& entities);
    void roll_initiative(std::vector<ecs::entity>& entities);

private:
    initiative_system m_initiative_system;

    encounter_state m_state = encounter_state::before_start;
};
