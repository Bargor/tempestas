set(GAME_SOURCES 
    main.cpp
)

tst_add_executable(tempestas 
    ""
    "${GAME_SOURCES}"
)

target_link_libraries(tempestas PRIVATE glfw fmt::fmt tst::application tst::rendering)
