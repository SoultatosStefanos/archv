// Contains a private module for the multithreading/workers header.
// Soultatos Stefanos 2022

#ifndef MULTITHREADING_DETAIL_WORKERS_HPP
#define MULTITHREADING_DETAIL_WORKERS_HPP

#include <boost/asio/thread_pool.hpp>
#include <thread>

namespace multithreading::detail
{

inline auto thread_pool() -> auto&
{
    static const auto nthreads = std::thread::hardware_concurrency();
    static auto pool = boost::asio::thread_pool(nthreads);
    return pool;
}

} // namespace multithreading::detail

#endif // MULTITHREADING_DETAIL_WORKERS_HPP
