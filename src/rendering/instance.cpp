#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#include "instance.h"

#include "utils.h"

#include <GLFW/glfw3.h>
#include <cstring>
#include <fmt/printf.h>

namespace tst::rendering {

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

namespace {
#ifndef NDEBUG

    inline std::string severity_to_string(vk::DebugUtilsMessageSeverityFlagBitsEXT severity) {
        switch (severity) {
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
            return "VERBOSE";
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
            return "INFO";
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
            return "WARNING";
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    inline std::string type_to_string(vk::DebugUtilsMessageTypeFlagsEXT type) {
        std::string result;

        if (type & vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral) result += "GENERAL|";
        if (type & vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation) result += "VALIDATION|";
        if (type & vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance) result += "PERFORMANCE|";

        if (!result.empty())
            result.pop_back();
        else
            result = "UNKNOWN";

        return result;
    }

    VKAPI_ATTR vk::Bool32 VKAPI_CALL debug_callback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                    vk::DebugUtilsMessageTypeFlagsEXT message_type,
                                                    const vk::DebugUtilsMessengerCallbackDataEXT* data,
                                                    void*) {
        fmt::printf("[Vulkan][%s][%s] %s\n",
                    severity_to_string(severity).c_str(),
                    type_to_string(message_type).c_str(),
                    data->pMessage);
        return vk::False;
    }
#endif

    std::vector<const char*> get_required_extensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        if (glfwExtensions == nullptr) {
            fmt::printf("Can't get required Vulkan extensions!\n");
            std::exit(EXIT_FAILURE);
        }

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    std::vector<const char*> get_required_validation_layers() {
#ifndef NDEBUG
        const std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

        // ---- Check availability ----
        auto result = vkTry(vk::enumerateInstanceLayerProperties());

        if (!result) [[unlikely]] {
            fmt::printf("Failed to enumerate instance layers: %s\n", vk::to_string(result.error()).c_str());
            std::exit(EXIT_FAILURE);
        }

        const auto& availableLayers = result.value();

        for (const char* layerName : layers) {
            bool found = false;

            for (const auto& layer : availableLayers) {
                if (std::strcmp(layer.layerName, layerName) == 0) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                fmt::printf("Validation layer not found: %s\n", layerName);
                std::exit(EXIT_FAILURE);
            }
        }

        return layers;
#else
        return {};
#endif
    }

    vk::UniqueInstance create_instance() {

        VULKAN_HPP_DEFAULT_DISPATCHER.init(::vkGetInstanceProcAddr);

        vk::ApplicationInfo appInfo("Tempestas",              // pApplicationName
                                    VK_MAKE_VERSION(0, 0, 1), // applicationVersion
                                    "Tempest",                // pEngineName
                                    VK_MAKE_VERSION(0, 0, 1), // engineVersion
                                    VK_API_VERSION_1_4        // apiVersion
        );

        auto extensions = get_required_extensions();
        auto layers = get_required_validation_layers();

        for (auto ext : extensions) {
            fmt::printf("Enabled extension: %s\n", ext);
        }

        vk::InstanceCreateInfo createInfo({},                                       // flags
                                          &appInfo,                                 // pApplicationInfo
                                          static_cast<uint32_t>(layers.size()),     // enabledLayerCount
                                          layers.data(),                            // ppEnabledLayerNames
                                          static_cast<uint32_t>(extensions.size()), // enabledExtensionCount
                                          extensions.data()                         // ppEnabledExtensionNames
        );

        // ---- Create instance ----
        auto result = vkTry(vk::createInstanceUnique(createInfo));

        if (!result) [[unlikely]] {
            fmt::printf("Failed to create instance: %s\n", vk::to_string(result.error()).c_str());
            std::abort();
        }

        auto instance = std::move(result.value());

        VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.get());

        return instance;
    }
} // namespace

const instance& instance::get_instance() noexcept {
    static instance instance;
    return instance;
}

instance::instance() : m_instance(create_instance()) {
#ifndef NDEBUG
    setup_debug_messenger();
#endif
}

#ifndef NDEBUG
void instance::setup_debug_messenger() {
    vk::PFN_DebugUtilsMessengerCallbackEXT cb = static_cast<vk::PFN_DebugUtilsMessengerCallbackEXT>(debug_callback);

    vk::DebugUtilsMessengerCreateInfoEXT createInfo(
        {}, // flags
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
        cb);

    auto result = vkTry(m_instance->createDebugUtilsMessengerEXTUnique(createInfo));

    if (!result) [[unlikely]] {
        fmt::printf("Failed to create debug messenger: %s\n", vk::to_string(result.error()).c_str());
        return;
    }

    m_debug_messenger = std::move(result.value());
}
#endif

} // namespace tst::rendering