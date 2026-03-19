add_executable(tempestas
    main.cpp
)

target_include_directories(tempestas PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(tempestas PRIVATE glfw fmt::fmt project_warnings)
