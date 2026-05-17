set(APPLICATION_SOURCES
    application.cpp
    arg_parser.cpp
    window.cpp
    main_window.cpp
    glfw_context.cpp
    glfw_window.cpp
)

set(APPLICATION_HEADERS
    application.h
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
        fmt::fmt
)
