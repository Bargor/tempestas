set(CORE_HEADERS
    assert.h
    container/queue.h
    core.h
    memory/frame_allocator.h
    platform.h
    position.h
    rectangle.h
    util/hash.h
    util/type_name.h
    util/variant.h
)

tst_add_interface_library(core
    "${CORE_HEADERS}"
)

target_link_libraries(core PUBLIC fmt::fmt)
