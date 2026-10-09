set(RENDERING_SOURCES 
    instance.cpp
)

set(RENDERING_HEADERS
    instance.h
    utils.h
)

tst_add_library(rendering 
    "${RENDERING_HEADERS}"
    "${RENDERING_SOURCES}"
)

target_include_directories(rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(rendering PUBLIC VULKAN_HPP_NO_EXCEPTIONS VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL=1)

target_link_libraries(rendering
    PUBLIC Vulkan::Vulkan
    PRIVATE tst::core glfw fmt::fmt
)
