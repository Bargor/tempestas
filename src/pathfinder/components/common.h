#pragma once

#include <core/util/flags.h>

#include <cstdint>
#include <vector>

namespace tst::pathfinder {

enum class proficency_rank : uint8_t {
    untrained,
    trained,
    expert,
    master,
    legendary
};

enum class language_flag_bits : uint32_t {
    common = 0x0001,
    dwarven = 0x0002,
    elven = 0x0004,
    gnomish = 0x0008,
    goblin = 0x0010,
    halfling = 0x0020,
    orcish = 0x0040,
    abyssal = 0x0080,
    draconic = 0x0100,
    infernal = 0x0200,
};

using language_flags = core::flags<language_flag_bits>;

struct stats {
    int32_t strength;
    int32_t dexterity;
    int32_t constitution;
    int32_t intelligence;
    int32_t wisdom;
    int32_t charisma;
};

struct perception {
    int32_t perception;
    proficency_rank rank;
    int32_t wisdom;
    int32_t item_bonus;
};

struct hp {
    uint32_t current;
    uint32_t max;
    uint32_t temp;
};

struct skills {
    int32_t acrobatics;
    int32_t arcana;
    int32_t athletics;
    int32_t crafting;
    int32_t deception;
    int32_t diplomacy;
    int32_t intimidation;
    int32_t investigation;
    int32_t medicine;
    int32_t nature;
    int32_t occultism;
    int32_t performance;
    int32_t religion;
    int32_t society;
    int32_t stealth;
    int32_t survival;
    int32_t thievery;
};

} // namespace tst::pathfinder