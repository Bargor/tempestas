set(CORE_HEADERS
    assert.h
    container/queue.h
    core.h
    platform.h
    position.h
    rectangle.h
    util/hash.h
    util/variant.h
)

add_library(core INTERFACE ${CORE_HEADERS})

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "Header Files"
    FILES ${CORE_HEADERS}
)

add_library(tst::core ALIAS core)

target_include_directories(core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(core INTERFACE cxx_std_23)

target_link_libraries(core
    INTERFACE
        project_warnings
)
