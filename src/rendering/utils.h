#include <vulkan/vulkan.hpp>

namespace tst::rendering {
template<typename T>
class vk_expected {
public:
    vk_expected(vk::Result result, T&& value) : m_result(result), m_value(std::move(value)) {
    }

    vk_expected(vk::Result result) : m_result(result) {
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
vk_expected<T> vk_try(vk::ResultValue<T>&& rv) {
    if (rv.result == vk::Result::eSuccess) [[likely]] {
        return vk_expected<T>(rv.result, std::move(rv.value));
    }
    return vk_expected<T>(rv.result);
}

} // namespace tst::rendering
