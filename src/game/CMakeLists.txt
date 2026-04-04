set(GAME_SOURCES 
    main.cpp
)

tst_add_executable(tempestas 
    ""
    "${GAME_SOURCES}"
)

target_include_directories(tempestas PRIVATE ${CMAKE_SOURCE_DIR}/third_party/glfw/include)
target_include_directories(tempestas PRIVATE ${Vulkan_INCLUDE_DIRS})

target_link_libraries(tempestas PRIVATE glfw fmt::fmt tst::rendering)
