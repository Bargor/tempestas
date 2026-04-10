set(APPLICATION_SOURCES
    arg_parser.cpp
)

set(APPLICATION_HEADERS
    arg_parser.h
)

tst_add_library(application
    "${APPLICATION_HEADERS}"
    "${APPLICATION_SOURCES}"
)
