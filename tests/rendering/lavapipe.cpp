#include <cstring>
#include <gtest/gtest.h>
#include <vulkan/vulkan.h>

bool is_lavapipe(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(device, &props);

    // Check multiple indicators for robustness
    const bool name_match = std::strstr(props.deviceName, "llvmpipe") != nullptr;
    const bool vendor_match = props.vendorID == 0x10005; // Mesa vendor ID
    const bool cpu_device = props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU;

    return name_match || (vendor_match && cpu_device);
}

TEST(vulkan_test, uses_lavapipe) {
    VkInstance instance;

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "lavapipe_test";
    app_info.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    ASSERT_EQ(vkCreateInstance(&create_info, nullptr, &instance), VK_SUCCESS);

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
    ASSERT_GT(device_count, 0u);

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    const bool found_lavapipe = [&devices]() {
        for (const auto device : devices) {
            if (is_lavapipe(device)) {
                return true;
            }
        }
        return false;
    }();

    EXPECT_TRUE(found_lavapipe) << "Lavapipe (llvmpipe) not detected";

    vkDestroyInstance(instance, nullptr);
}
