// Contains some multithreading workers utilities.
// Soultatos Stefanos 2022

#ifndef MULTITHREADING_WORKER_HPP
#define MULTITHREADING_WORKER_HPP

#include "detail/workers.hpp"

#include <boost/asio/post.hpp>
#include <concepts>

namespace multithreading
{

// From a thread pool.
template < std::invocable Task >
inline auto spawn_worker(Task&& task) -> void
{
    using boost::asio::post;
    post(detail::thread_pool(), [token = std::move(task)]() { token(); });
}

} // namespace multithreading

#endif // MULTITHREADING_WORKER_HPP
