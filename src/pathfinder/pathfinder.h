#pragma once

#include "encounter/encounter.h"
#include "session.h"
#include "util/dice_roller.h"

namespace tst::ecs {
class registry;
}

namespace tst::pathfinder {

class pathfinder {
public:
    pathfinder(ecs::registry& registry, uint32_t seed);

    session new_session(ecs::registry& registry);
    session load_session();
    void save_session();
    void close_session();

private:
    void init_system();

    void register_components();
    void load_data();

private:
    ecs::registry& m_registry;
    dice_roller m_roller;
    encounter_system m_encounter_system;
};

} // namespace tst::pathfinder