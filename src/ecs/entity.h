#pragma once

#include <cstdint>

namespace tst::ecs {

using entity_id = uint32_t;

struct entity {
    entity_id id;
    uint32_t generation;
    bool operator==(const entity& other) const = default;
};

} // namespace tst::ecs