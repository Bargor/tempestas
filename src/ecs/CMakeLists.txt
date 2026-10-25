set(ECS_SOURCES
    registry.cpp
)

set(ECS_HEADERS
    entity.h
    registry.h
    sparse_set.h
)

tst_add_library(ecs
    "${ECS_HEADERS}"
    "${ECS_SOURCES}"
)

target_link_libraries(ecs
    PUBLIC
        tst::core
)
