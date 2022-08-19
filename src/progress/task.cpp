#include "task.hpp"

#include <thread>

namespace progress
{

// TODO: thread pool?
auto launch_task(task& t, task::units todo) -> void
{
    auto worker = std::thread([&t, todo]() { t.work(todo); });
    worker.detach();
}

auto launch_task(task& t) -> void { launch_task(t, t.total_units()); }

} // namespace progress