#include <application/event.h>
#include <application/event_processor.h>
#include <chrono>
#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#error "Application tests must compile without C++ exception support"
#endif

namespace tst::application {

using namespace std::chrono_literals;

TEST(event_processor, dispatches_matching_events_in_queue_order) {
    event_processor<event> processor;
    std::vector<event::mouse_position> received_events;

    processor.subscribe<event::mouse_position>([&received_events](const event& value) {
        const auto* position = std::get_if<event::mouse_position>(&value.data);
        ASSERT_NE(position, nullptr);
        received_events.push_back(*position);
    });
    processor.subscribe<event::scroll>([](const event&) { FAIL() << "Received an event of the wrong type"; });

    EXPECT_TRUE(processor.create_event(event{nullptr, event::mouse_position{1.0, 2.0}}));
    EXPECT_TRUE(processor.create_event(event{nullptr, event::mouse_position{3.0, 4.0}}));

    processor.process_events(0us);
    ASSERT_EQ(received_events.size(), 2);
    EXPECT_DOUBLE_EQ(received_events[0].x, 1.0);
    EXPECT_DOUBLE_EQ(received_events[0].y, 2.0);
    EXPECT_DOUBLE_EQ(received_events[1].x, 3.0);
    EXPECT_DOUBLE_EQ(received_events[1].y, 4.0);
}

TEST(event_processor, supplies_the_complete_event_when_requested) {
    event_processor<event> processor;
    const int source = 42;
    const void* received_source = nullptr;

    processor.subscribe<event::closed>([&received_source](const event& value) { received_source = value.source; });

    EXPECT_TRUE(processor.create_event(event{&source, event::closed{}}));
    processor.process_events(0us);

    EXPECT_EQ(received_source, &source);
}

TEST(event_processor, throttles_subscriber_updates) {
    event_processor<event> processor;
    std::vector<std::chrono::microseconds> received_times;

    processor.subscribe<event::time>(
        [&received_times](const event& value) {
            const auto* time = std::get_if<event::time>(&value.data);
            ASSERT_NE(time, nullptr);
            received_times.push_back(time->value);
        },
        10ms);

    EXPECT_TRUE(processor.create_event(event{nullptr, event::time{1ms}}));
    processor.process_events(9ms);
    EXPECT_TRUE(received_times.empty());

    EXPECT_TRUE(processor.create_event(event{nullptr, event::time{2ms}}));
    processor.process_events(1ms);
    ASSERT_EQ(received_times.size(), 1);
    EXPECT_EQ(received_times[0], 2ms);

    EXPECT_TRUE(processor.create_event(event{nullptr, event::time{3ms}}));
    processor.process_events(9ms);
    EXPECT_EQ(received_times.size(), 1);

    EXPECT_TRUE(processor.create_event(event{nullptr, event::time{4ms}}));
    processor.process_events(1ms);
    ASSERT_EQ(received_times.size(), 2);
    EXPECT_EQ(received_times[1], 4ms);
}

TEST(event_processor, periodic_subscription_receives_one_event_per_interval) {
    event_processor<event> processor;
    std::vector<std::chrono::microseconds> received_times;

    processor.subscribe<event::time>(
        [&received_times](const event& value) {
            const auto* time = std::get_if<event::time>(&value.data);
            ASSERT_NE(time, nullptr);
            received_times.push_back(time->value);
        },
        10ms);

    EXPECT_TRUE(processor.create_event(event{nullptr, event::time{1ms}}));
    EXPECT_TRUE(processor.create_event(event{nullptr, event::time{2ms}}));
    processor.process_events(10ms);

    ASSERT_EQ(received_times.size(), 1);
    EXPECT_EQ(received_times[0], 1ms);
}

TEST(event_processor, zero_frequency_receives_every_update) {
    event_processor<event> processor;
    size_t callback_count = 0;

    processor.subscribe<event::closed>([&callback_count](const event&) { ++callback_count; });

    EXPECT_TRUE(processor.create_event(event{nullptr, event::closed{}}));
    processor.process_events(0us);
    EXPECT_TRUE(processor.create_event(event{nullptr, event::closed{}}));
    processor.process_events(0us);

    EXPECT_EQ(callback_count, 2);
}

TEST(event_processor, unrelated_events_do_not_reset_update_interval) {
    event_processor<event> processor;
    size_t callback_count = 0;

    processor.subscribe<event::time>([&callback_count](const event&) { ++callback_count; }, 10ms);

    EXPECT_TRUE(processor.create_event(event{nullptr, event::scroll{}}));
    processor.process_events(10ms);
    EXPECT_EQ(callback_count, 0);

    EXPECT_TRUE(processor.create_event(event{nullptr, event::time{}}));
    processor.process_events(0us);
    EXPECT_EQ(callback_count, 1);
}

TEST(event_processor, unsubscribe_stops_callbacks) {
    event_processor<event> processor;
    size_t callback_count = 0;

    const auto subscription = processor.subscribe<event::closed>([&callback_count](const event&) { ++callback_count; });

    EXPECT_TRUE(processor.unsubscribe(subscription));
    EXPECT_FALSE(processor.unsubscribe(subscription));
    EXPECT_TRUE(processor.create_event(event{nullptr, event::closed{}}));
    processor.process_events(0us);

    EXPECT_EQ(callback_count, 0);
}

TEST(event_processor, subscription_id_encodes_event_type_and_vector_index) {
    event_processor<event> processor;
    constexpr auto event_type_count = std::variant_size_v<event::payload>;

    const auto first_mouse_position = processor.subscribe<event::mouse_position>([](const event&) {});
    const auto scroll = processor.subscribe<event::scroll>([](const event&) {});
    const auto second_mouse_position = processor.subscribe<event::mouse_position>([](const event&) {});

    EXPECT_EQ(first_mouse_position, 0);
    EXPECT_EQ(scroll, 2);
    EXPECT_EQ(second_mouse_position, event_type_count);
}

TEST(event_processor, queue_leaves_one_slot_open_and_wraps_around) {
    event_processor<event> processor;
    size_t callback_count = 0;
    constexpr size_t maximum_event_count = event_processor<event>::queue_capacity - 1;

    processor.subscribe<event::closed>([&callback_count](const event&) { ++callback_count; });

    for (size_t index = 0; index < maximum_event_count; ++index) {
        EXPECT_TRUE(processor.create_event(event{nullptr, event::closed{}}));
    }
    EXPECT_FALSE(processor.create_event(event{nullptr, event::closed{}}));
    processor.process_events(0us);
    EXPECT_EQ(callback_count, maximum_event_count);

    for (size_t index = 0; index < maximum_event_count; ++index) {
        EXPECT_TRUE(processor.create_event(event{nullptr, event::closed{}}));
    }
    processor.process_events(0us);
    EXPECT_EQ(callback_count, maximum_event_count * 2);
}

} // namespace tst::application
