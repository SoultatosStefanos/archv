// Contains a class responsible for connecting the graph backend side with the
// frontend one.
// Soultatos Stefanos 2022

#ifndef APPLICATION_GRAPH_PRESENTER_HPP
#define APPLICATION_GRAPH_PRESENTER_HPP

#include "architecture/all.hpp"
#include "dependencies/all.hpp"
#include "graph_interface.hpp"
#include "gui/all.hpp"
#include "layout/all.hpp"
#include "rendering/graph_renderer.hpp"
#include "scaling/all.hpp"
#include "undo_redo/allfwd.hpp"

namespace application
{

class graph_presenter
{
public:
    using graph_renderer_type = rendering::graph_renderer<
        graph_interface::graph_type,
        graph_interface::id_map,
        graph_interface::dependency_map >;

    using dependencies_editor_type = gui::dependencies_editor;
    using layout_editor_type = gui::layout_editor;
    using scaling_editor_type = gui::scaling_editor;
    using configurator_type = gui::graph_configurator;

    using command_history_type = undo_redo::command_history;

    graph_presenter(
        graph_interface& g,
        graph_renderer_type& renderer,
        dependencies_editor_type& deps_editor,
        layout_editor_type& layout_editor,
        scaling_editor_type& scaling_editor,
        configurator_type& configurator,
        command_history_type& cmds);

    auto graph_iface() const -> const auto& { return m_g_iface; }
    auto graph_iface() -> auto& { return m_g_iface; }

    auto renderer() const -> const auto& { return m_g_renderer; }
    auto renderer() -> auto& { return m_g_renderer; }

    auto dependencies_editor() const -> const auto& { return m_deps_editor; }
    auto dependencies_editor() -> auto& { return m_deps_editor; }

    auto layout_editor() const -> const auto& { return m_layout_editor; }
    auto layout_editor() -> auto& { return m_layout_editor; }

    auto scaling_editor() const -> const auto& { return m_scaling_editor; }
    auto scaling_editor() -> auto& { return m_scaling_editor; }

    auto configurator() const -> const auto& { return m_configurator; }
    auto configurator() -> auto& { return m_configurator; }

    auto commands() const -> const auto& { return m_cmds; }
    auto commands() -> auto& { return m_cmds; }

private:
    auto prepare_dependencies_editor() -> void;
    auto prepare_layout_editor() -> void;
    auto prepare_scaling_editor() -> void;
    auto prepare_configurator() -> void;

    auto connect_dependencies_editor_with_graph_iface() -> void;
    auto connect_layout_editor_with_graph_iface() -> void;
    auto connect_scaling_editor_with_graph_iface() -> void;

    auto connect_graph_iface_with_renderer() -> void;

    auto connect_configurator_with_renderer() -> void;

    graph_interface& m_g_iface;
    graph_renderer_type& m_g_renderer;
    dependencies_editor_type& m_deps_editor;
    layout_editor_type& m_layout_editor;
    scaling_editor_type& m_scaling_editor;
    configurator_type& m_configurator;
    command_history_type& m_cmds;
};

} // namespace application

#endif // APPLICATION_GRAPH_PRESENTER_HPP
