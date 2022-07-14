// Contains the core of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CORE_HPP
#define LAYOUT_CORE_HPP

#include "controller.hpp"
#include "events.hpp"
#include "layout.hpp"
#include "presenter.hpp"
#include "services.hpp"
#include "utility/event_system.hpp"
#include "utility/undo_redo.hpp"
#include "view.hpp"

#include <memory>

namespace layout
{

class core final
{
public:
    using event_bus = utility::event_bus;
    using graph = architecture::graph;
    using command_history = utility::command_history;

    core(const core&) = delete;
    core(core&&) = delete;

    auto operator=(const core&) -> core& = delete;
    auto operator=(core&&) -> core& = delete;

    static auto get() -> core&
    {
        static core singleton;
        return singleton;
    }

    void initialize(command_history& cmds,
                    const std::string& layout_type,
                    const std::string& topology_type,
                    double topology_scale,
                    const graph& g,
                    const Ogre::SceneManager& scene);

    auto pipeline() const -> const event_bus& { return m_pipeline; }
    auto pipeline() -> event_bus& { return m_pipeline; }

private:
    using core_controller =
        controller<update_layout_service, update_topology_service>;
    using core_presenter = presenter<view>;
    using layout_pointer = layout_factory::pointer;

    core() = default;
    ~core() = default;

    void initialize_topology(const std::string& topology_type, double scale);
    void initialize_layout(const std::string& layout_type, const graph& g);

    void initialize_mvp(command_history& cmds,
                        const graph& g,
                        const Ogre::SceneManager& scene);

    event_bus m_pipeline;

    layout_pointer m_layout;
    topology m_space;

    std::unique_ptr<view> m_view;
    std::unique_ptr<core_controller> m_controller;
    std::unique_ptr<core_presenter> m_presenter;
};

} // namespace layout

#endif // LAYOUT_CORE_HPP
