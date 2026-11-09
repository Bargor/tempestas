#pragma once

#include <array>
#include <cstdint>

namespace tst::core {

template<typename E, typename T, size_t N>
    requires std::is_enum_v<E>
class enum_array {
    constexpr T& operator[](E index) noexcept {
        return m_array[static_cast<size_t>(index)];
    }

    constexpr const T& operator[](E index) const noexcept {
        return m_array[static_cast<size_t>(index)];
    }

    constexpr void fill(E value) {
        m_array.fill(static_cast<T>(value));
    }

    constexpr std::size_t size() const noexcept {
        return m_array.size();
    }

    constexpr T* data() noexcept {
        return m_array.data();
    }
    constexpr const T* data() const noexcept {
        return m_array.data();
    }

    constexpr auto begin() noexcept {
        return m_array.begin();
    }
    constexpr auto begin() const noexcept {
        return m_array.begin();
    }

    constexpr auto end() noexcept {
        return m_array.end();
    }
    constexpr auto end() const noexcept {
        return m_array.end();
    }

    constexpr T& front() noexcept {
        return m_array.front();
    }
    constexpr const T& front() const noexcept {
        return m_array.front();
    }

    constexpr T& back() noexcept {
        return m_array.back();
    }
    constexpr const T& back() const noexcept {
        return m_array.back();
    }

private:
    std::array<T, N> m_array{};
};

} // namespace tst::core
