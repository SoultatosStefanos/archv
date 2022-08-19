#include "async.hpp"

#include <thread>

namespace progress
{

// TODO: thread pool?
auto launch_async_task(task& t, task::units todo) -> void
{
    auto worker = std::thread([&t, todo]() { t.work(todo); });
    worker.detach();
}

} // namespace progress