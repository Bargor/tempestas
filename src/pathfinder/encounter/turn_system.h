#pragma once

#include "game_event.h"
#include "util/dice_roller.h"

#include <ecs/entity.h>
#include <ecs/sparse_set.h>
#include <vector>

namespace tst::pathfinder {

enum class round_phase {
    round_start,
    round_end,
    turn_start,
    regain_resources,
    main_turn,
    turn_end
};

struct turn_resources {
    size_t actions = 3;
    size_t reactions = 1;
    size_t free_actions = 1;
};

class turn_system {
public:
    struct phase_data {
        round_phase phase;
        size_t round;
        std::vector<game_event> events;
    };

public:
    turn_system(ecs::sparse_set<perception>& perception_set, dice_roller& roller);

    void roll_initiative(std::vector<ecs::entity>& entities);
    void next_turn();

private:
    ecs::sparse_set<perception>& m_perception_set;
    dice_roller& m_roller;

    std::vector<ecs::entity> m_initiative_order;
    std::vector<phase_data> m_phases;

    size_t m_round = 0;
    size_t m_current_index = 0;
};

} // namespace tst::pathfinder
