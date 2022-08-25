// Contains a class responsibe for launching progess monitored tasks on worker
// threads.
// Soultatos Stefanos 2022

#ifndef APPLICATION_TASK_MANAGER_HPP
#define APPLICATION_TASK_MANAGER_HPP

#include "architecture/generation.hpp"
#include "gui/allfwd.hpp"
#include "progress/allfwd.hpp"

#include <boost/asio/io_service.hpp>
#include <jsoncpp/json/json.h>
#include <memory>

namespace application
{

// Async.
class task_manager
{
public:
    using arch_generation = architecture::generation;

    using symbol_table = architecture::symbol_table;
    using graph = architecture::graph;
    using vertex_properties = arch_generation::vertex_properties;
    using overlay_manager = gui::overlay_manager;

    task_manager(
        boost::asio::io_service& io,
        symbol_table& st,
        graph& g,
        vertex_properties& props,
        overlay_manager& mngr);

    // Monitored via progress bars tasks.
    auto launch_arch_generation(const Json::Value& root) -> void;

private:
    boost::asio::io_service& m_io;

    symbol_table& m_st;
    graph& m_g;
    vertex_properties& m_vertex_props;
    overlay_manager& m_overlays;

    std::unique_ptr< gui::progress_bar > m_arch_bar;
};

} // namespace application

#endif // APPLICATION_TASK_MANAGER_HPP
