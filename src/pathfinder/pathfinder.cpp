#include "pathfinder.h"

#include <ecs/registry.h>

namespace tst::pathfinder {

pathfinder::pathfinder(ecs::registry& registry) : m_registry(registry) {
}

} // namespace tst::pathfinder