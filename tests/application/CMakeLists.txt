set(APPLICATION_TEST_SOURCES
    window_test.cpp
    glfw_window_test.cpp
)

tst_add_executable(application_test
    ""
    "${APPLICATION_TEST_SOURCES}"
)

target_link_libraries(application_test
    PRIVATE
        GTest::gtest_main
        tst::application
)

add_test(NAME application.tests COMMAND application_test)
