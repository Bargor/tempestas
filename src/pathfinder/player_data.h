#pragma once

#include "components/common.h"

#include <cstdint>
#include <vector>

namespace tst::pathfinder {

struct player_data {
    stats ability_scores;
    perception perception_score;

    language_flags languages;

    uint32_t speed;
    hp hit_points;

    skills skills;
};

} // namespace tst::pathfinder