#pragma once

#include <string>

namespace tst::ecs {
class registry;
}

namespace tst::pathfinder {

// Represents a game session, which manages the state of the game and its entities.

class session {
private:
    session(ecs::registry& registry);

public:
    static session new_session(ecs::registry& registry) {
        return session(registry);
    }

    static session load_session(ecs::registry& registry, const std::string&) {
        // TODO: Load the session from a save file (not implemented)
        return session(registry);
    }

    void save_session(const std::string&) {
        // TODO: Save the session to a save file (not implemented)
        return;
    }

    void start_transaction() noexcept;

private:
    ecs::registry& m_registry;
};

} // namespace tst::pathfinder
