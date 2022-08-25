// Contains a private module for the multithreading workers utilities.
// Soultatos Stefanos 2022

#ifndef MULTITHREADING_DETAIL_WORKERS_HPP
#define MULTITHREADING_DETAIL_WORKERS_HPP

#include <boost/asio/thread_pool.hpp>
#include <thread>

namespace multithreading::detail
{

auto thread_pool() -> auto&
{
    static const auto nthreads = std::thread::hardware_concurrency();
    static auto pool = boost::asio::thread_pool(nthreads);
    [[maybe_unused]] static const auto finally = []() { pool.join(); };

    return pool;
}

} // namespace multithreading::detail

#endif // MULTITHREADING_DETAIL_WORKERS_HPP
