#include "task_manager.hpp"

#include "gui/all.hpp"
#include "multithreading/all.hpp"
#include "progress/all.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace application
{

task_manager::task_manager(
    boost::asio::io_service& io,
    symbol_table& st,
    graph& g,
    vertex_properties& props,
    overlay_manager& mngr)
: m_io { io }
, m_st { st }
, m_g { g }
, m_vertex_props { props }
, m_overlays { mngr }
, m_arch_bar { std::make_unique< gui::progress_bar >() }
{
    m_arch_bar->caption() = "Generating 3D Architecture...";
}

namespace
{
    inline auto
    monitor(const progress::task& task, gui::progress_bar& progress_bar)
    {
        progress_bar.progress() = progress::percentage_done(task);
        assert(progress_bar.progress() >= 0);
        assert(progress_bar.progress() <= 100);
    }

} // namespace

auto task_manager::launch_arch_generation(const Json::Value& root) -> void
{
    assert(m_arch_bar);

    auto task = std::make_unique< arch_generation >(root);

    task->connect([this](const auto& task) { monitor(task, *m_arch_bar); });

    m_overlays.submit(m_arch_bar.get());

    multithreading::spawn_worker(
        [task = std::move(task), this]()
        {
            progress::work_all_units_at_once(*task);

            assert(progress::finished(*task));

            boost::asio::post(
                m_io,
                [res = task->result(), this]()
                {
                    m_overlays.withdraw(m_arch_bar.get());
                    std::tie(m_st, m_g, m_vertex_props) = std::move(res);
                });
        });
}

} // namespace application