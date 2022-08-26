// Contains utilites for inter thread message passing.
// Soultatos Stefanos 2022

#ifndef MULTITHREADING_MESSAGES_HPP
#define MULTITHREADING_MESSAGES_HPP

#include "detail/messages.hpp"

#include <boost/asio/post.hpp>
#include <concepts>

namespace multithreading
{

// Thread safe.
template < std::invocable Message >
inline auto post_message(Message msg) -> void
{
    using boost::asio::post;
    post(detail::io(), [token = std::move(msg)]() { token(); });
}

// Thread safe.
inline auto poll_message() -> void { detail::io().poll_one(); }

} // namespace multithreading

#endif // MULTITHREADING_MESSAGES_HPP
