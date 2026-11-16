set(PATHFINDER_SOURCES 
    util/dice_roller.cpp
)

set(PATHFINDER_HEADERS
    util/dice_roller.h
)

tst_add_library(pathfinder 
    "${PATHFINDER_HEADERS}"
    "${PATHFINDER_SOURCES}"
)

target_include_directories(pathfinder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(pathfinder
    PRIVATE tst::core tst::ecs fmt::fmt
)
