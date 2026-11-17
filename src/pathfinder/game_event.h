#pragma once

#include <variant>
#include <cstdint>

namespace tst::pathfinder {
struct game_event {
    // Define the properties of a game event

    using payload = std::variant <>;

    payload data{};
    // Assigned by event_processor when the event is queued. Zero means not queued.
    uint64_t event_id{};
};

static_assert(std::is_default_constructible_v<game_event>, "event must be default-constructible");
static_assert(std::is_trivially_copy_constructible_v<game_event>, "event must be trivially copy-constructible");
static_assert(std::is_trivially_copy_assignable_v<game_event>, "event must be trivially copy-assignable");

} // namespace tst::pathfinder