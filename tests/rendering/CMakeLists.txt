set(RENDERING_TEST_SOURCES
    lavapipe.cpp
)

tst_add_executable(lavapipe_test
    ""
    "${RENDERING_TEST_SOURCES}"
)

target_link_libraries(lavapipe_test
    PRIVATE
        GTest::gtest_main
        Vulkan::Vulkan
)

add_test(NAME rendering.lavapipe COMMAND lavapipe_test)
