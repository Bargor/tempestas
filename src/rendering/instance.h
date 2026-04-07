#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst::rendering {

class instance {
    friend class engine;
    friend class device;

public:
    static const instance& get_instance() noexcept;

public:
    const vk::UniqueInstance& get_instance_handle() const noexcept {
        return m_instance;
    }

private:
    instance();
    ~instance() = default;

    instance(const instance&) = delete;
    instance& operator=(const instance&) = delete;

    void setup_debug_messenger();

private:
    vk::UniqueInstance m_instance;
#ifndef NDEBUG
    vk::UniqueDebugUtilsMessengerEXT m_debug_messenger;
#endif
};

} // namespace tst::rendering
