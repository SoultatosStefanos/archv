// Contains a general purpose progress monitored interface.
// Soultatos Stefanos 2022

#ifndef PROGRESS_TASK_HPP
#define PROGRESS_TASK_HPP

#include <boost/signals2/signal.hpp>

namespace progress
{

class task
{
    using signal = boost::signals2::signal< void(const task&) >;

public:
    using units = unsigned;
    using monitor = signal::slot_type;
    using connection = boost::signals2::connection;

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

    auto connect(const monitor& f) -> connection { return m_signal.connect(f); }

protected:
    auto emit_status() const -> void { m_signal(*this); }

private:
    signal m_signal;
};

inline auto percentage_done(const task& t) -> task::units
{
    return (t.units_done() * 100) / t.total_units();
}

inline auto work_all_units_at_once(task& t) -> void { t.work(t.total_units()); }

} // namespace progress

#endif // PROGRESS_TASK_HPP
