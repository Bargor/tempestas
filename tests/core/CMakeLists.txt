set(CORE_TEST_SOURCES
    container/queue_test.cpp
    geometry_test.cpp
)

tst_add_executable(core_test
    ""
    "${CORE_TEST_SOURCES}"
)

target_link_libraries(core_test
    PRIVATE
        GTest::gtest_main
        tst::core
)

add_test(NAME core.tests COMMAND core_test)
