#pragma once

#include "core.h"

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace tst {
namespace core {

    // Single producer/single consumer lock-free queue based on ring buffer.

    template<typename T, size_t Size>
    class spsc_queue {
    public:
        using index_type = int32_t;

    public:
        spsc_queue() noexcept;

        [[nodiscard]] bool try_push(const T& element) noexcept;
        [[nodiscard]] std::optional<T> try_pop() noexcept;

    private:
        static constexpr index_type next_pos(index_type index) noexcept;

    private:
        static constexpr size_t buffer_size = Size + 1;
        std::array<T, buffer_size> m_buffer{};
        std::atomic<index_type> m_front;
        std::atomic<index_type> m_back;
    };

    template<typename T, size_t Size>
    TST_INLINE spsc_queue<T, Size>::spsc_queue() noexcept : m_front(0), m_back(0) {
    }

    template<typename T, size_t Size>
    TST_INLINE bool spsc_queue<T, Size>::try_push(const T& element) noexcept {
        const index_type front_pos = m_front.load(std::memory_order_relaxed);
        const index_type new_front = next_pos(front_pos);

        if (new_front == m_back.load(std::memory_order_acquire)) {
            // Queue is full.
            return false;
        }

        m_buffer[front_pos] = element;
        m_front.store(new_front, std::memory_order_release);

        return true;
    }

    template<typename T, size_t Size>
    TST_INLINE std::optional<T> spsc_queue<T, Size>::try_pop() noexcept {
        const index_type back_pos = m_back.load(std::memory_order_relaxed);
        const index_type front_pos = m_front.load(std::memory_order_acquire);

        if (front_pos == back_pos) {
            // Queue is empty.
            return std::nullopt;
        }

        auto value = m_buffer[back_pos];
        m_back.store(next_pos(back_pos), std::memory_order_release);

        return value;
    }

    template<typename T, size_t Size>
    constexpr typename spsc_queue<T, Size>::index_type spsc_queue<T, Size>::next_pos(index_type index) noexcept {
        return ++index == static_cast<index_type>(buffer_size) ? 0 : index;
    }

} // namespace core
} // namespace tst
