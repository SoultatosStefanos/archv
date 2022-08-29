// Contains a module which contains async workers utilites.
// Soultatos Stefanos 2022

#ifndef MULTITHREADING_WORKERS_HPP
#define MULTITHREADING_WORKERS_HPP

#include "detail/workers.hpp"

#include <boost/asio/post.hpp>
#include <concepts>

namespace multithreading
{

// Detached from a thread pool.
template < std::invocable Task >
inline auto launch_worker(Task task) -> void
{
    using boost::asio::post;
    post(detail::thread_pool(), [token = std::move(task)]() { token(); });
}

} // namespace multithreading

#endif // MULTITHREADING_WORKERS_HPP
