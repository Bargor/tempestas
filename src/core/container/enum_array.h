#pragma once

#include <array>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace tst::core {

template<typename E, typename T, size_t N>
    requires std::is_enum_v<E>
class enum_array {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = typename std::array<T, N>::iterator;
    using const_iterator = typename std::array<T, N>::const_iterator;
    using reverse_iterator = typename std::array<T, N>::reverse_iterator;
    using const_reverse_iterator = typename std::array<T, N>::const_reverse_iterator;

    constexpr enum_array() = default;

    template<typename... Args>
        requires(sizeof...(Args) == N) && (std::is_convertible_v<Args, T> && ...)
    constexpr enum_array(Args&&... args) : m_array{{static_cast<T>(std::forward<Args>(args))...}} {
    }

    constexpr T& operator[](E index) noexcept {
        return m_array[static_cast<size_t>(index)];
    }

    constexpr const T& operator[](E index) const noexcept {
        return m_array[static_cast<size_t>(index)];
    }

    constexpr reference at(E index) {
        return m_array.at(static_cast<size_type>(index));
    }

    constexpr const_reference at(E index) const {
        return m_array.at(static_cast<size_type>(index));
    }

    constexpr void fill(E value) {
        m_array.fill(static_cast<T>(value));
    }

    constexpr std::size_t size() const noexcept {
        return m_array.size();
    }

    constexpr size_type max_size() const noexcept {
        return m_array.max_size();
    }

    constexpr bool empty() const noexcept {
        return m_array.empty();
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

    constexpr auto cbegin() const noexcept {
        return m_array.cbegin();
    }

    constexpr auto end() noexcept {
        return m_array.end();
    }
    constexpr auto end() const noexcept {
        return m_array.end();
    }

    constexpr auto cend() const noexcept {
        return m_array.cend();
    }

    constexpr auto rbegin() noexcept {
        return m_array.rbegin();
    }

    constexpr auto rbegin() const noexcept {
        return m_array.rbegin();
    }

    constexpr auto crbegin() const noexcept {
        return m_array.crbegin();
    }

    constexpr auto rend() noexcept {
        return m_array.rend();
    }

    constexpr auto rend() const noexcept {
        return m_array.rend();
    }

    constexpr auto crend() const noexcept {
        return m_array.crend();
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
