#include <vulkan/vulkan.hpp>

namespace tst::rendering {
template<typename T>
class VkExpected {
public:
    VkExpected(vk::Result result, T&& value) : m_result(result), m_value(std::move(value)) {
    }

    VkExpected(vk::Result result) : m_result(result) {
    }

    explicit operator bool() const {
        return m_result == vk::Result::eSuccess;
    }

    const T& value() const {
        return m_value;
    }

    T& value() {
        return m_value;
    }

    vk::Result error() const {
        return m_result;
    }

private:
    vk::Result m_result;
    T m_value;
};

template<typename T>
VkExpected<T> vkTry(vk::ResultValue<T>&& rv) {
    if (rv.result == vk::Result::eSuccess) [[likely]] {
        return VkExpected<T>(rv.result, std::move(rv.value));
    }
    return VkExpected<T>(rv.result);
}

} // namespace tst::rendering