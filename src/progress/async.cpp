#include "async.hpp"

#include <cassert>
#include <thread>

namespace progress
{

// TODO: thread pool?
auto launch_async_task(std::unique_ptr< task > t, task::units todo) -> void
{
    assert(t);
    auto worker = std::thread([j = std::move(t), todo]() { j->work(todo); });
    worker.detach();
}

} // namespace progress