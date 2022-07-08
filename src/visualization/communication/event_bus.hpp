// Contains a general purpose message passing medium.
// Soultatos Stefanos 2022

#ifndef COMMUNICATION_EVENT_BUS_HPP
#define COMMUNICATION_EVENT_BUS_HPP

#include <algorithm>
#include <any>
#include <cassert>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace visualization::communication
{

// General purpose messaging system medium.
class event_bus
{
public:
    template <typename Event>
    using subscriber = std::function<void(const Event&)>;

    template <typename Event>
    void subscribe(subscriber<Event> f)
    {
        assert(f);

        auto&& index = typeid(Event);
        auto&& subs = m_table[index];
        subs.push_back(std::move(f));

        assert(m_table.contains(index));
    }

    template <typename Event>
    void post(const Event& e) const
    {
        auto&& index = typeid(Event);

        if (m_table.contains(index))
        {
            auto&& subs = m_table.at(index);
            assert(!subs.empty());

            for (const auto& any : subs)
            {
                auto&& sub = std::any_cast<subscriber<Event>>(any);
                std::invoke(sub, e);
            }
        }
    }

private:
    using subscribers = std::vector<std::any>;
    using subscriber_table = std::unordered_map<std::type_index, subscribers>;

    subscriber_table m_table;
};

} // namespace visualization::communication

#endif // COMMUNICATION_EVENT_BUS_HPP
