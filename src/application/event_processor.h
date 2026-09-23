#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <chrono>
#include <concepts>
#include <core/util/variant.h>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace tst::application {

template<typename Event>
concept processable_event = std::default_initializable<Event> && std::is_trivially_copy_constructible_v<Event> &&
    std::is_trivially_copy_assignable_v<Event> && requires(Event value) {
        typename Event::payload;
        requires core::is_variant_v<typename Event::payload>;
        requires std::same_as<std::remove_cvref_t<decltype(value.data)>, typename Event::payload>;
    };

template<processable_event Event>
class event_processor {
public:
    using duration = std::chrono::microseconds;
    using subscription_id = uint64_t;

    static constexpr size_t queue_capacity = 256;

    // Calling subscribe from an event callback is prohibited because processing iterates subscription vectors directly.
    template<typename EventSubtype>
    subscription_id subscribe(std::function<void(const Event&)> callback, duration update_frequency = duration::zero());

    [[nodiscard]] bool unsubscribe(subscription_id id);

    // Calling create_event from an event callback is prohibited because it mutates the queue being processed.
    [[nodiscard]] bool create_event(const Event& event);

    void process_events();
    void process_events(duration elapsed_time);

private:
    struct subscription {
        std::function<void(const Event&)> callback{};
        duration update_frequency{};
        duration last_update_time{};
        bool active{true};
    };

    static_assert(std::has_single_bit(queue_capacity));
    static constexpr size_t event_type_count = std::variant_size_v<typename Event::payload>;

    // Keep fewer than queue_capacity events queued so equal read and write indices always mean empty.
    // One array slot is intentionally left unused, giving the queue a maximum size of 255 events.
    std::array<Event, queue_capacity> m_event_queue{};
    std::array<std::vector<subscription>, event_type_count> m_subscriptions{};
    size_t m_read_index{};
    size_t m_write_index{};
    duration m_current_time{};
    std::chrono::steady_clock::time_point m_last_process_time{std::chrono::steady_clock::now()};
};

template<processable_event Event>
template<typename EventSubtype>
typename event_processor<Event>::subscription_id
event_processor<Event>::subscribe(std::function<void(const Event&)> callback, duration update_frequency) {
    constexpr size_t subtype_index = core::variant_index<EventSubtype, typename Event::payload>();

    auto& subscriptions = m_subscriptions[subtype_index];
    // Encode the subtype bucket in the remainder and the stable vector slot in the quotient.
    // unsubscribe reverses this using modulo and division by event_type_count; slots are never erased.
    const subscription_id id = static_cast<subscription_id>(subtype_index + subscriptions.size() * event_type_count);
    subscriptions.push_back(subscription{
        .callback = std::move(callback),
        .update_frequency = update_frequency,
        .last_update_time = m_current_time,
    });

    return id;
}

template<processable_event Event>
bool event_processor<Event>::unsubscribe(subscription_id id) {
    const size_t event_type = static_cast<size_t>(id % event_type_count);
    const size_t subscription_index = static_cast<size_t>(id / event_type_count);
    auto& subscriptions = m_subscriptions[event_type];

    if (subscription_index >= subscriptions.size() || !subscriptions[subscription_index].active) return false;

    subscriptions[subscription_index].active = false;
    subscriptions[subscription_index].callback = {};
    return true;
}

template<processable_event Event>
bool event_processor<Event>::create_event(const Event& event) {
    const size_t next_write_index = (m_write_index + 1) & (queue_capacity - 1);
    if (next_write_index == m_read_index) {
        return false;
    }

    m_event_queue[m_write_index] = event;
    m_write_index = next_write_index;
    return true;
}

template<processable_event Event>
void event_processor<Event>::process_events() {
    const auto current_time = std::chrono::steady_clock::now();
    const auto elapsed_time = std::chrono::duration_cast<duration>(current_time - m_last_process_time);
    m_last_process_time = current_time;
    process_events(elapsed_time);
}

template<processable_event Event>
void event_processor<Event>::process_events(duration elapsed_time) {
    elapsed_time = std::max(elapsed_time, duration::zero());
    m_current_time += elapsed_time;

    while (m_read_index != m_write_index) {
        Event event = m_event_queue[m_read_index];
        m_read_index = (m_read_index + 1) & (queue_capacity - 1);

        const size_t event_type = event.data.index();
        assert(event_type < event_type_count);

        auto& subscriptions = m_subscriptions[event_type];
        for (auto& subscription : subscriptions) {
            const bool is_due = m_current_time - subscription.last_update_time >= subscription.update_frequency;
            if (!subscription.active || !is_due) continue;

            auto callback = subscription.callback;
            subscription.last_update_time = m_current_time;
            callback(event);
        }
    }
}

} // namespace tst::application
