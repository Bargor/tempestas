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

target_include_directories(rendering PRIVATE ${CMAKE_SOURCE_DIR}/third_party/glfw/include)
target_include_directories(rendering PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(rendering PRIVATE VULKAN_HPP_NO_EXCEPTIONS VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL=1)

target_link_libraries(rendering PRIVATE glfw fmt::fmt Vulkan::Vulkan)