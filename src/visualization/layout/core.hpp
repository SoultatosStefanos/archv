// Contains the core of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CORE_HPP
#define LAYOUT_CORE_HPP

#include "command/all.hpp"
#include "controller.hpp"
#include "events.hpp"
#include "layout.hpp"
#include "presenter.hpp"
#include "services.hpp"
#include "view.hpp"
#include "visualization/communication/all.hpp"

#include <memory>

namespace visualization::layout
{

class core final
{
public:
    using event_bus = communication::event_bus;
    using graph = architecture::graph;
    using command_history = command::command_history;

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
    core() = default;
    ~core() = default;

    void initialize_topology(const std::string& topology_type, double scale);
    void initialize_layout(const std::string& layout_type, const graph& g);

    void initialize_mvp(command_history& cmds,
                        const std::string& layout_type,
                        const std::string& topology_type,
                        double scale,
                        const graph& g,
                        const Ogre::SceneManager& scene);

    event_bus m_pipeline;

    std::unique_ptr<layout> m_layout;
    topology m_space;

    std::unique_ptr<view> m_view;
    std::unique_ptr<controller> m_controller;
    std::unique_ptr<presenter> m_presenter;
};

} // namespace visualization::layout

#endif // LAYOUT_CORE_HPP
