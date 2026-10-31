#pragma once

#include "components/common.h"

#include <cstdint>
#include <vector>

namespace tst::pathfinder {

struct npc_data {
    stats ability_scores;

    language_flags languages;

    uint32_t speed;
    hp hit_points;

    skills skills;
};

} // namespace tst::pathfinder