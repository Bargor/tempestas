#pragma once

#include <ecs/registry.h>

namespace tst::pathfinder {

// Represents a game session, which manages the state of the game and its entities.

class session {
public:
    session(ecs::registry& registry);

private:
    ecs::registry& m_registry;
}

} // namespace tst::pathfinder
