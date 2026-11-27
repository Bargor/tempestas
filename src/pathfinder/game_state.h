#pragma once

#include <ecs/registry.h>

namespace tst::pathfinder {

class game_state {
public:
    game_state(ecs::registry& registry);

    // Add methods to manage the game state here (transactions)

private:
    ecs::registry& m_registry;
};

} // namespace tst::pathfinder
