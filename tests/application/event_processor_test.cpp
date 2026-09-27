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
    int source{};
    std::vector<event::mouse_position> received_events;

    processor.subscribe<event::mouse_position>(
        nullptr, [&received_events](const event::mouse_position& position) { received_events.push_back(position); }, 0us);
    processor.subscribe<event::scroll>(
        nullptr, [](const event::scroll&) { FAIL() << "Received an event of the wrong type"; }, 0us);

    EXPECT_TRUE(processor.create_event(event::mouse_position{1.0, 2.0}, &source));
    EXPECT_TRUE(processor.create_event(event::mouse_position{3.0, 4.0}, &source));

    processor.process_events(0us);
    ASSERT_EQ(received_events.size(), 2);
    EXPECT_DOUBLE_EQ(received_events[0].x, 1.0);
    EXPECT_DOUBLE_EQ(received_events[0].y, 2.0);
    EXPECT_DOUBLE_EQ(received_events[1].x, 3.0);
    EXPECT_DOUBLE_EQ(received_events[1].y, 4.0);
}

TEST(event_processor, skips_the_source_subscriber) {
    event_processor<event> processor;
    int source{};
    int other_subscriber{};
    size_t source_callback_count{};
    size_t other_callback_count{};
    size_t anonymous_callback_count{};

    processor.subscribe<event::closed>(
        &source, [&source_callback_count](const event::closed&) { ++source_callback_count; }, 0us);
    processor.subscribe<event::closed>(
        &other_subscriber, [&other_callback_count](const event::closed&) { ++other_callback_count; }, 0us);
    processor.subscribe<event::closed>(
        nullptr, [&anonymous_callback_count](const event::closed&) { ++anonymous_callback_count; }, 0us);

    EXPECT_TRUE(processor.create_event(event::closed{}, &source));
    EXPECT_TRUE(processor.create_event(event::closed{}, &source));
    processor.process_events(0us);

    EXPECT_EQ(source_callback_count, 0);
    EXPECT_EQ(other_callback_count, 2);
    EXPECT_EQ(anonymous_callback_count, 2);
}

TEST(event_processor, throttles_subscriber_updates) {
    event_processor<event> processor;
    int source{};
    std::vector<std::chrono::microseconds> received_times;

    processor.subscribe<event::time>(
        nullptr, [&received_times](const event::time& time) { received_times.push_back(time.value); }, 10ms);

    EXPECT_TRUE(processor.create_event(event::time{1ms}, &source));
    processor.process_events(9ms);
    EXPECT_TRUE(received_times.empty());

    EXPECT_TRUE(processor.create_event(event::time{2ms}, &source));
    processor.process_events(1ms);
    ASSERT_EQ(received_times.size(), 1);
    EXPECT_EQ(received_times[0], 2ms);

    EXPECT_TRUE(processor.create_event(event::time{3ms}, &source));
    processor.process_events(9ms);
    EXPECT_EQ(received_times.size(), 1);

    EXPECT_TRUE(processor.create_event(event::time{4ms}, &source));
    processor.process_events(1ms);
    ASSERT_EQ(received_times.size(), 2);
    EXPECT_EQ(received_times[1], 4ms);
}

TEST(event_processor, periodic_subscription_receives_one_event_per_interval) {
    event_processor<event> processor;
    int source{};
    std::vector<std::chrono::microseconds> received_times;

    processor.subscribe<event::time>(
        nullptr, [&received_times](const event::time& time) { received_times.push_back(time.value); }, 10ms);

    EXPECT_TRUE(processor.create_event(event::time{1ms}, &source));
    EXPECT_TRUE(processor.create_event(event::time{2ms}, &source));
    processor.process_events(10ms);

    ASSERT_EQ(received_times.size(), 1);
    EXPECT_EQ(received_times[0], 1ms);
}

TEST(event_processor, zero_frequency_receives_every_update) {
    event_processor<event> processor;
    int source{};
    size_t callback_count = 0;

    processor.subscribe<event::closed>(nullptr, [&callback_count](const event::closed&) { ++callback_count; }, 0us);

    EXPECT_TRUE(processor.create_event(event::closed{}, &source));
    processor.process_events(0us);
    EXPECT_TRUE(processor.create_event(event::closed{}, &source));
    processor.process_events(0us);

    EXPECT_EQ(callback_count, 2);
}

TEST(event_processor, unrelated_events_do_not_reset_update_interval) {
    event_processor<event> processor;
    int source{};
    size_t callback_count = 0;

    processor.subscribe<event::time>(nullptr, [&callback_count](const event::time&) { ++callback_count; }, 10ms);

    EXPECT_TRUE(processor.create_event(event::scroll{}, &source));
    processor.process_events(10ms);
    EXPECT_EQ(callback_count, 0);

    EXPECT_TRUE(processor.create_event(event::time{}, &source));
    processor.process_events(0us);
    EXPECT_EQ(callback_count, 1);
}

TEST(event_processor, unsubscribe_stops_callbacks) {
    event_processor<event> processor;
    int source{};
    size_t callback_count = 0;

    const auto subscription =
        processor.subscribe<event::closed>(nullptr, [&callback_count](const event::closed&) { ++callback_count; }, 0us);

    EXPECT_TRUE(processor.unsubscribe(subscription));
    EXPECT_FALSE(processor.unsubscribe(subscription));
    EXPECT_TRUE(processor.create_event(event::closed{}, &source));
    processor.process_events(0us);

    EXPECT_EQ(callback_count, 0);
}

TEST(event_processor, subscription_id_encodes_event_type_and_vector_index) {
    event_processor<event> processor;
    constexpr auto event_type_count = std::variant_size_v<event::payload>;

    const auto first_mouse_position =
        processor.subscribe<event::mouse_position>(nullptr, [](const event::mouse_position&) {}, 0us);
    const auto scroll = processor.subscribe<event::scroll>(nullptr, [](const event::scroll&) {}, 0us);
    const auto second_mouse_position =
        processor.subscribe<event::mouse_position>(nullptr, [](const event::mouse_position&) {}, 0us);

    EXPECT_EQ(first_mouse_position, 0);
    EXPECT_EQ(scroll, 2);
    EXPECT_EQ(second_mouse_position, event_type_count);
}

TEST(event_processor, queue_leaves_one_slot_open_and_wraps_around) {
    event_processor<event> processor;
    int source{};
    size_t callback_count = 0;
    constexpr size_t maximum_event_count = event_processor<event>::queue_capacity - 1;

    processor.subscribe<event::closed>(nullptr, [&callback_count](const event::closed&) { ++callback_count; }, 0us);

    for (size_t index = 0; index < maximum_event_count; ++index) {
        EXPECT_TRUE(processor.create_event(event::closed{}, &source));
    }
    EXPECT_FALSE(processor.create_event(event::closed{}, &source));
    processor.process_events(0us);
    EXPECT_EQ(callback_count, maximum_event_count);

    for (size_t index = 0; index < maximum_event_count; ++index) {
        EXPECT_TRUE(processor.create_event(event::closed{}, &source));
    }
    processor.process_events(0us);
    EXPECT_EQ(callback_count, maximum_event_count * 2);
}

} // namespace tst::application
