// Contains some async task management utilities.
// Soultatos Stefanos 2022

#ifndef PROGRESS_ASYNC_HPP
#define PROGRESS_ASYNC_HPP

#include "task.hpp"

namespace progress
{

auto launch_async_task(task& t, task::units todo) -> void;

inline auto launch_async_task(task& t) -> void
{
    launch_async_task(t, t.total_units());
}

} // namespace progress

#endif // PROGRESS_ASYNC_HPP
