#include "session.h"

#include <ecs/registry.h>

namespace tst::pathfinder {

session::session(ecs::registry& registry) : m_registry(registry) {
}
} // namespace tst::pathfinder
