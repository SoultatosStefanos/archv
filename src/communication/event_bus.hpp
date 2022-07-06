// Contains a general purpose message passing medium.
// Soultatos Stefanos 2022

#ifndef COMMUNICATION_EVENT_BUS_HPP
#define COMMUNICATION_EVENT_BUS_HPP

#include "event.hpp"

#include <any>
#include <cassert>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace communication
{

// General purpose messaging system medium.
class event_bus
{
public:
    template <typename Event>
    using subscriber = std::function<void(const Event&)>;

    template <message Event>
    void subscribe(subscriber<Event> f)
    {
        auto&& desc = event_traits<Event>::desc();
        m_table[desc].push_back(std::move(f));

        assert(m_table.contains(desc));
    }

    template <message Event>
    void post(const Event& e) const
    {
        auto&& desc = event_traits<Event>::desc();

        if (m_table.contains(desc))
        {
            auto&& subscribed = m_table.at(desc);

            assert(!subscribed.empty());

            for (const auto& any : subscribed)
            {
                assert(any.has_value());
                assert(any.type() == typeid(e));

                std::invoke(std::any_cast<subscriber<Event>>(any), e)
            }
        }
    }

private:
    using subscribers = std::vector<std::any>;
    using subscriber_table = std::unordered_map<event::descriptor, subscribers>;

    subscriber_table m_table;
};

// Global event pipeline.
extern event_bus pipeline;

} // namespace communication

#endif // COMMUNICATION_EVENT_BUS_HPP
