set(PATHFINDER_SOURCES
    encounter/encounter.cpp
    encounter/turn_system.cpp
    combat.cpp
    database.cpp
    pathfinder.cpp
    session.cpp
    util/dice_roller.cpp
    event_log.cpp
    session.cpp
)

set(PATHFINDER_HEADERS
    components/common.h
    components/npc_data.h
    components/player_data.h
    encounter/encounter.h
    encounter/turn_system.h
    combat.h
    database.h
    pathfinder.h
    session.h
    util/dice_roller.h
    event_log.h
    game_event.h
    session.h
)

tst_add_library(pathfinder 
    "${PATHFINDER_HEADERS}"
    "${PATHFINDER_SOURCES}"
)

target_include_directories(pathfinder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(pathfinder
    PRIVATE tst::core tst::ecs fmt::fmt
)
