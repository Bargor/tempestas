#pragma once

#include <cstdint>
#include <limits>

namespace tst::ecs {

using entity_id = uint32_t;

struct entity {
    entity_id id;
    uint32_t generation;
    bool operator==(const entity& other) const = default;
};

static constexpr entity invalid_entity = {std::numeric_limits<entity_id>::max(), 0};

} // namespace tst::ecs