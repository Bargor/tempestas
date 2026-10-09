set(DEVICE_SOURCES
    monitor.cpp
)

set(DEVICE_HEADERS
    monitor.h
    keyboard.h
    mouse.h
)

tst_add_library(device
    "${DEVICE_HEADERS}"
    "${DEVICE_SOURCES}"
)

target_link_libraries(device PUBLIC tst::core glfw)
