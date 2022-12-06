#ifndef PRESENTATION_DEFFWD_HPP
#define PRESENTATION_DEFFWD_HPP

#include "architecture/allfwd.hpp"
#include "architecture/graph.hpp" // would not want to fwd declare boost::detail
#include "clustering/allfwd.hpp"
#include "gui/allfwd.hpp"
#include "layout/allfwd.hpp"
#include "rendering/allfwd.hpp"
#include "scaling/allfwd.hpp"
#include "undo_redo/allfwd.hpp"
#include "weights/allfwd.hpp"

namespace presentation
{

using graph = architecture::graph;
using symbol_table = architecture::symbol_table;
using vertex_marker = architecture::vertex_marker;
using metadata_counter = architecture::metadata_counter;

using id_map = architecture::id_map;
using dependency_map = architecture::dependency_map;
using weight_map = weights::weight_map< graph, dependency_map >;
using scale_map = scaling::scale_map< graph, metadata_counter >;

using weights_backend = weights::backend;
using layout_backend = layout::backend< graph, weight_map >;
using scaling_backend = scaling::backend;
using degrees_backend = rendering::degrees_ranked_backend;
using clustering_backend = clustering::backend< graph, weight_map >;

using cluster_map = clustering::cluster_map< clustering_backend >;
using position_map = layout::position_map< layout_backend >;

using id_t = std::string;
using vertex = std::size_t;

using command_history = undo_redo::command_history;

using background_renderer = rendering::background_renderer;

using graph_renderer = rendering::graph_renderer<
    architecture::graph,
    architecture::id_map,
    architecture::dependency_map,
    rendering::degrees_ranked_evaluator,
    rendering::cluster_color_pool >;

using graph_collision_checker = rendering::graph_collision_checker<
    architecture::graph,
    architecture::id_map,
    architecture::dependency_map >;

using minimap_renderer = rendering::minimap_renderer;

using menu_bar = gui::menu_bar;
using weights_editor = gui::weights_editor;
using layout_editor = gui::layout_editor;
using scaling_editor = gui::scaling_editor;
using degrees_editor = gui::degrees_editor;
using clustering_editor = gui::clustering_editor;
using controls_hud = gui::controls_hud;
using frames_hud = gui::frames_hud;
using overlay_manager = gui::overlay_manager;
using popup_holder = gui::popup_holder;

} // namespace presentation

#endif // PRESENTATION_DEFFWD_HPP
