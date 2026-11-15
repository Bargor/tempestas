#pragma once

#include "components/common.h"
#include "game_event.h"
#include "util/dice_roller.h"

#include <ecs/entity.h>
#include <ecs/sparse_set.h>

#include <vector>

#include "game_event.h"

namespace tst::pathfinder {

enum class round_phase {
    round_start,
    round_end,
    turn_start,
    regain_resources,
    main_turn,
    turn_end,
    enum_size,
};

struct turn_resources {
    size_t actions = 3;
    size_t reactions = 1;
    size_t free_actions = 1;
};

class turn_system {
public:
    struct phase_data {
        ecs::entity entity = ecs::invalid_entity;
        round_phase phase;
        size_t round;
        std::vector<game_event> events;
    };

    struct initiative_data {
        ecs::entity entity;
        int32_t initiative;
    };

public:
    turn_system(ecs::sparse_set<perception>& perception_set, dice_roller& roller);

    void roll_initiative(std::vector<ecs::entity>& entities);
    ecs::entity next_turn();

    const std::vector<initiative_data>& get_initiative_order() const noexcept;
    const phase_data& get_current_phase() const;

    void finish_encounter() noexcept;

private:
    static constexpr size_t m_default_rounds = 10;

    ecs::sparse_set<perception>& m_perception_set;
    dice_roller& m_roller;

    std::vector<initiative_data> m_initiative_order;
    std::vector<phase_data> m_phases;

    size_t m_round = 0;
    size_t m_current_index = 0;

    bool m_encounter_active = false;
};

} // namespace tst::pathfinder
