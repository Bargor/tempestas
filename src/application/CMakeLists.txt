set(APPLICATION_SOURCES
    arg_parser.cpp
    window.cpp
    glfw_context.cpp
    glfw_window.cpp
)

set(APPLICATION_HEADERS
    arg_parser.h
    window.h
    main_window.h
    glfw_context.h
    glfw_window.h
)

tst_add_library(application
    "${APPLICATION_HEADERS}"
    "${APPLICATION_SOURCES}"
)

target_link_libraries(application
    PUBLIC
        tst::core
        tst::device
        glfw
)
