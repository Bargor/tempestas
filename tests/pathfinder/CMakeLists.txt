set(PATHFINDER_TEST_SOURCES
    dice_roller_test.cpp
    turn_system_test.cpp
)

tst_add_executable(pathfinder_test
    ""
    "${PATHFINDER_TEST_SOURCES}"
)

target_link_libraries(pathfinder_test
    PRIVATE
        GTest::gtest_main
        tst::core
        tst::pathfinder
)

add_test(NAME pathfinder.tests COMMAND pathfinder_test)
