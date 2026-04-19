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

target_include_directories(device PRIVATE ${CMAKE_SOURCE_DIR}/third_party/glfw/include)

target_link_libraries(device PUBLIC glfw)
