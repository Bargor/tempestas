set(ECS_TEST_SOURCES
    sparse_set_test.cpp
)

tst_add_executable(ecs_test
    ""
    "${ECS_TEST_SOURCES}"
)

target_link_libraries(ecs_test
    PRIVATE
        GTest::gtest_main
        tst::ecs
)

add_test(NAME ecs.tests COMMAND ecs_test)
