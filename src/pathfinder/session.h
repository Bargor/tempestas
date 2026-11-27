#pragma once

#include "encounter/encounter.h"
#include "game_state.h"

#include <ecs/entity.h>
#include <string>
#include <vector>

namespace tst::ecs {
class registry;
}

namespace tst::pathfinder {

class dice_roller;

// Represents a game session, which manages the state of the game and its susbsystems.

class session {
private:
    session(ecs::registry& registry, dice_roller& roller);

public:
    static session new_session(ecs::registry& registry, dice_roller& roller) {
        return session(registry, roller);
    }

    static session load_session(ecs::registry& registry, dice_roller& roller, const std::string&) {
        // TODO: Load the session from a save file (not implemented)
        return session(registry, roller);
    }

    void save_session(const std::string&) {
        // TODO: Save the session to a save file (not implemented)
        return;
    }

    void close_session();

private:
    ecs::registry& m_registry;
    dice_roller& m_roller;

    game_state m_game_state;
    encounter_system m_encounter_system;
};

} // namespace tst::pathfinder
