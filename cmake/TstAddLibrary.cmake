include(${PROJECT_SOURCE_DIR}/cmake/ProjectWarnings.cmake)

function(tst_add_library NAME HEADERS SOURCES)
    add_library(${NAME} STATIC)

    add_library(tst::${NAME} ALIAS ${NAME})

    target_sources(${NAME} PUBLIC ${HEADERS} PRIVATE ${SOURCES})

    target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
    target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    # Require C++ standard
    target_compile_features(${NAME} PUBLIC cxx_std_20)

    # Link warnings (your existing target)
    target_link_libraries(${NAME}
        PRIVATE
            project_warnings
    )

    # Group headers
    source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        PREFIX "Header Files"
        FILES ${HEADERS}
    )

    # Group sources
    source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        PREFIX "Source Files"
        FILES ${SOURCES}
    )

    target_compile_options(${NAME} PRIVATE "$<$<CONFIG:Debug>:${TST_DEBUG_COMPILE_OPTIONS}>")
    target_compile_options(${NAME} PRIVATE "$<$<CONFIG:Release>:${TST_RELEASE_COMPILE_OPTIONS}>")
    target_compile_options(${NAME} PRIVATE "$<$<CONFIG:RelWithDebInfo>:${TST_RELWITHDEBINFO_COMPILE_OPTIONS}>")

    target_compile_definitions(${NAME} PRIVATE "$<$<CONFIG:Debug>:${TST_DEBUG_COMPILE_DEFINITIONS}>")
    target_compile_definitions(${NAME} PRIVATE "$<$<CONFIG:Release>:${TST_RELEASE_COMPILE_DEFINITIONS}>")
    target_compile_definitions(${NAME} PRIVATE "$<$<CONFIG:RelWithDebInfo>:${TST_RELWITHDEBINFO_COMPILE_DEFINITIONS}>")

    target_link_options(${NAME} PRIVATE $<$<CONFIG:Debug>:${TST_DEBUG_LINK_OPTIONS}> $<$<CONFIG:Release>:${TST_RELEASE_LINK_OPTIONS}>)

endfunction()

function(tst_add_executable NAME HEADERS SOURCES)
    add_executable(${NAME})

    add_executable(tst::${NAME} ALIAS ${NAME})

    target_sources(${NAME}
        PRIVATE ${SOURCES}
        PUBLIC  ${HEADERS}
    )

    target_include_directories(${NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_compile_features(${NAME}
        PUBLIC cxx_std_20
    )

    target_link_libraries(${NAME}
        PRIVATE project_warnings
    )

    # Source groups
    source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        PREFIX "Header Files"
        FILES ${HEADERS}
    )

    source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        PREFIX "Source Files"
        FILES ${SOURCES}
    )

    # Compile options
    target_compile_options(${NAME} PRIVATE
        $<$<CONFIG:Debug>:${TST_DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${TST_RELEASE_COMPILE_OPTIONS}>
        $<$<CONFIG:RelWithDebInfo>:${TST_RELWITHDEBINFO_COMPILE_OPTIONS}>
    )

    # Compile definitions
    target_compile_definitions(${NAME} PRIVATE
        $<$<CONFIG:Debug>:${TST_DEBUG_COMPILE_DEFINITIONS}>
        $<$<CONFIG:Release>:${TST_RELEASE_COMPILE_DEFINITIONS}>
        $<$<CONFIG:RelWithDebInfo>:${TST_RELWITHDEBINFO_COMPILE_DEFINITIONS}>
    )

    # Link options
    target_link_options(${NAME} PRIVATE
        $<$<CONFIG:Debug>:${TST_DEBUG_LINK_OPTIONS}>
        $<$<CONFIG:Release>:${TST_RELEASE_LINK_OPTIONS}>
    )

endfunction()
