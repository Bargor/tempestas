add_library(core INTERFACE)

add_library(tst::core ALIAS core)

target_include_directories(core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(core INTERFACE cxx_std_20)

target_link_libraries(core
    INTERFACE
        project_warnings
)
