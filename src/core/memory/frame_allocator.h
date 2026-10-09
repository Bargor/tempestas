#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <memory_resource>
#include <new>

namespace tst {
namespace core {

    // Fixed-capacity, thread-safe bump resource. Exhaustion terminates the process
    // because project code does not use exceptions and PMR cannot return null.
    class frame_allocator final : public std::pmr::memory_resource {
    public:
        static constexpr std::size_t cache_line_size = 64;

        explicit frame_allocator(std::size_t size) noexcept : m_size(size) {
            m_buffer = static_cast<std::byte*>(
                ::operator new(size == 0 ? 1 : size, std::align_val_t(cache_line_size), std::nothrow));
            if (m_buffer == nullptr) {
                std::abort();
            }
        }

        ~frame_allocator() override {
            ::operator delete(m_buffer, std::align_val_t(cache_line_size));
        }

        frame_allocator(const frame_allocator&) = delete;
        frame_allocator& operator=(const frame_allocator&) = delete;
        frame_allocator(frame_allocator&&) = delete;
        frame_allocator& operator=(frame_allocator&&) = delete;

        // Call only after all allocations and users of the previous frame have
        // finished. Invalidates every allocation; does not invoke destructors.
        void reset() noexcept {
            m_offset.store(0, std::memory_order_relaxed);
        }

    private:
        void* do_allocate(std::size_t bytes, std::size_t alignment) override {
            if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
                std::abort();
            }
            if (alignment < cache_line_size) {
                alignment = cache_line_size;
            }
            // Zero-sized allocations still consume space and have unique addresses.
            if (bytes == 0) {
                bytes = 1;
            }

            auto offset = m_offset.load(std::memory_order_relaxed);
            for (;;) {
                void* candidate = m_buffer + offset;
                auto left_space = m_size - offset;
                if (std::align(alignment, bytes, candidate, left_space) == nullptr) {
                    std::abort();
                }
                const auto aligned_offset = static_cast<std::size_t>(static_cast<std::byte*>(candidate) - m_buffer);
                const auto next_offset = aligned_offset + bytes;
                if (m_offset.compare_exchange_weak(offset, next_offset, std::memory_order_relaxed)) {
                    return candidate;
                }
            }
        }

        void do_deallocate(void*, std::size_t, std::size_t) override {
        }

        bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
            return this == &other;
        }

        std::byte* m_buffer = nullptr;
        const std::size_t m_size;
        std::atomic<std::size_t> m_offset{0};
    };

} // namespace core
} // namespace tst
