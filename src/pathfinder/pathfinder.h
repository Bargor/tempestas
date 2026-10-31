#pragma once

#include "session.h"

namespace tst::ecs {
class registry;
}

namespace tst::pathfinder {

class pathfinder {
public:
    pathfinder(ecs::registry& registry);

    session new_session(ecs::registry& registry);
    session load_session();
    void save_session();
    void close_session();

private:
    ecs::registry& m_registry;
};

} // namespace tst::pathfinder