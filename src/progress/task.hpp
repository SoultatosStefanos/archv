// Contains a general purpose progress monitored interface.
// Soultatos Stefanos 2022

#ifndef PROGRESS_TASK_HPP
#define PROGRESS_TASK_HPP

namespace progress
{

class task
{
public:
    using units = unsigned;

    task() = default;
    task(const task&) = default;
    task(task&&) = default;
    virtual ~task() = default;

    auto operator=(const task&) -> task& = default;
    auto operator=(task&&) -> task& = default;

    virtual auto total_units() const -> units = 0;
    virtual auto units_done() const -> units = 0;

    virtual auto stop() -> void = 0;
    virtual auto suspend() -> void = 0;
    virtual auto resume() -> void = 0;

    virtual auto work(units todo) -> void = 0;
};

inline auto work_all_units_at_once(task& t) -> void { t.work(t.total_units()); }

inline auto percentage_done(const task& t) -> task::units
{
    return (t.units_done() * 100) / t.total_units();
}

inline auto finished(const task& t) -> bool
{
    return t.total_units() == t.units_done();
}

} // namespace progress

#endif // PROGRESS_TASK_HPP
