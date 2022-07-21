// Contains a manager class of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CORE_HPP
#define LAYOUT_CORE_HPP

#include "factories.hpp"
#include "layout.hpp"
#include "presenter.hpp"
#include "services.hpp"
#include "topology.hpp"
#include "view.hpp"

#include <cassert>
#include <memory>

namespace features::layout
{

class core final
{
public:
    using command_history = utility::command_history;
    using graph = dependencies::graph;
    using weight_map = dependencies::weight_map;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using layout_descriptor = layout_factory::descriptor;
    using topology_descriptor = topology_factory::descriptor;
    using topology_scale = topology_factory::scale_type;

    using core_presenter =
        presenter<view, update_layout_service, update_topology_service>;

    static_assert(std::is_default_constructible_v<layout_pointer>);
    static_assert(std::is_default_constructible_v<topology_pointer>);

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
                    const graph& g,
                    weight_map edge_weight,
                    const Ogre::SceneManager& scene,
                    layout_descriptor layout_desc,
                    topology_descriptor topolgy_desc,
                    topology_scale scale);

    auto get_layout() const -> const layout&
    {
        assert(m_layout);
        return *m_layout;
    }

    auto get_layout() -> layout&
    {
        assert(m_layout);
        return *m_layout;
    }

    auto get_topology() const -> const topology&
    {
        assert(m_topology);
        return *m_topology;
    }

    auto get_topology() -> topology&
    {
        assert(m_topology);
        return *m_topology;
    }

    auto get_update_layout_service() const -> const update_layout_service&
    {
        assert(m_update_layout);
        return *m_update_layout;
    }

    auto get_update_layout_service() -> update_layout_service&
    {
        assert(m_update_layout);
        return *m_update_layout;
    }

    auto get_update_topology_service() const -> const update_topology_service&
    {
        assert(m_update_topology);
        return *m_update_topology;
    }

    auto get_update_topology_service() -> update_topology_service&
    {
        assert(m_update_topology);
        return *m_update_topology;
    }

    auto get_presenter() const -> const core_presenter&
    {
        assert(m_presenter);
        return *m_presenter;
    }

    auto get_presenter() -> core_presenter&
    {
        assert(m_presenter);
        return *m_presenter;
    }

    auto get_view() const -> const view&
    {
        assert(m_view);
        return *m_view;
    }

    auto get_view() -> view&
    {
        assert(m_view);
        return *m_view;
    }

private:
    core() = default;
    ~core() = default;

    layout_pointer m_layout;
    topology_pointer m_topology;

    std::unique_ptr<core_presenter> m_presenter;
    std::unique_ptr<view> m_view;
    std::unique_ptr<update_layout_service> m_update_layout;
    std::unique_ptr<update_topology_service> m_update_topology;
};

} // namespace features::layout

#endif // LAYOUT_CORE_HPP
