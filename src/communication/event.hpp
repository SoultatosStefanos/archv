// Defines a communication system message.
// Soultatos Stefanos 2022

#ifndef COMMUNICATION_EVENT_HPP
#define COMMUNICATION_EVENT_HPP

#include <concepts>
#include <memory>
#include <string_view>

namespace communication
{

// General purpose message passing proxy.
class event
{
public:
    using descriptor = std::string_view;

    event() = default;
    event(const event&) = default;
    event(event&&) = default;
    virtual ~event() = default;

    auto operator=(const event&) -> event& = default;
    auto operator=(event&&) -> event& = default;

    virtual auto clone() const -> std::unique_ptr<event> = 0;
};

// General purpose message type.
template <typename Class>
concept message = std::derived_from<Class, event> &&
    requires(typename Class::descriptor desc)
{
    // clang-format off
    {Class::desc} -> std::convertible_to<decltype(desc)>;
    // clang-format on
};

template <message Event>
struct event_traits
{
    using descriptor = typename Event::descriptor;
    static constexpr auto desc() -> descriptor { return Event::desc; }
};

} // namespace communication

#endif // COMMUNICATION_EVENT_HPP
