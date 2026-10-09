#include <algorithm>
#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <memory/frame_allocator.h>
#include <thread>
#include <vector>

using tst::core::frame_allocator;

TEST(frame_allocator, alignment_and_frame_reuse) {
    frame_allocator resource(1024);
    auto* first = resource.allocate(1);
    auto* second = resource.allocate(65);
    auto* aligned = resource.allocate(128, 128);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(first) % 64, 0u);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(second) % 64, 0u);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(aligned) % 128, 0u);
    EXPECT_GE(static_cast<std::byte*>(second) - static_cast<std::byte*>(first), 64);
    resource.deallocate(first, 1);
    EXPECT_NE(resource.allocate(1), first);
    resource.reset();
    EXPECT_EQ(resource.allocate(1024), first);
}

TEST(frame_allocator, pmr_container_and_identity) {
    frame_allocator resource(1024);
    frame_allocator other(1024);
    EXPECT_TRUE(resource.is_equal(resource));
    EXPECT_FALSE(resource.is_equal(other));
    {
        std::pmr::vector<int> values(&resource);
        values.reserve(32);
        for (int value = 0; value < 32; ++value) {
            values.push_back(value);
        }
        EXPECT_EQ(values.back(), 31);
    }
    resource.reset();
    EXPECT_NE(resource.allocate(1024), nullptr);
}

TEST(frame_allocator, concurrent_allocations_are_disjoint) {
    constexpr std::size_t thread_count = 8;
    constexpr std::size_t allocations_per_thread = 256;
    constexpr std::size_t allocation_count = thread_count * allocations_per_thread;
    frame_allocator resource(allocation_count * 64);
    std::array<std::uintptr_t, allocation_count> addresses{};
    std::array<std::thread, thread_count> workers;
    for (std::size_t index = 0; index < thread_count; ++index) {
        workers[index] = std::thread([&, index] {
            for (std::size_t allocation = 0; allocation < allocations_per_thread; ++allocation) {
                auto* pointer = resource.allocate(64);
                static_cast<std::byte*>(pointer)[63] = std::byte{42};
                addresses[index * allocations_per_thread + allocation] = reinterpret_cast<std::uintptr_t>(pointer);
            }
        });
    }
    for (auto& worker : workers) {
        worker.join();
    }
    std::sort(addresses.begin(), addresses.end());
    for (std::size_t index = 0; index < allocation_count; ++index) {
        EXPECT_EQ(addresses[index] % 64, 0u);
        if (index != 0) {
            EXPECT_EQ(addresses[index] - addresses[index - 1], 64u);
        }
    }
    resource.reset();
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(resource.allocate(allocation_count * 64)), addresses.front());
}

TEST(frame_allocator, zero_size_allocations_consume_space) {
    frame_allocator resource(128);
    EXPECT_NE(resource.allocate(0), resource.allocate(0));
}

TEST(frame_allocator, exhaustion_terminates) {
    EXPECT_DEATH(
        {
            frame_allocator resource(0);
            (void)resource.allocate(1);
        },
        "");
    EXPECT_DEATH(
        {
            frame_allocator resource(64);
            (void)resource.allocate(65);
        },
        "");
    EXPECT_DEATH(
        {
            frame_allocator resource(64);
            (void)resource.allocate(1);
            (void)resource.allocate(1);
        },
        "");
    EXPECT_DEATH(
        {
            frame_allocator resource(64);
            // Keep the boundary value runtime-dependent so GCC does not reject
            // the deliberate oversized request with -Walloc-size-larger-than.
            const volatile std::size_t oversized_bytes = static_cast<std::size_t>(-1);
            (void)resource.allocate(oversized_bytes);
        },
        "");
}
