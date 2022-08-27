// Contains a private module for the multithreading/messages header.
// Soultatos Stefanos 2022

#ifndef MULTITHREADING_DETAIL_MESSAGES_HPP
#define MULTITHREADING_DETAIL_MESSAGES_HPP

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_service.hpp>

namespace multithreading::detail
{

// NOTE: The gui and any workers can share the same service.
inline auto io() -> auto&
{
    static auto service = boost::asio::io_service();
    static auto worker = boost::asio::make_work_guard(service);
    return service;
}

} // namespace multithreading::detail

#endif // MULTITHREADING_DETAIL_MESSAGES_HPP
