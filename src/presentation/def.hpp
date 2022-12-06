#ifndef PRESENTATION_DEF_HPP
#define PRESENTATION_DEF_HPP

#include "architecture/all.hpp"
#include "clustering/all.hpp"
#include "gui/all.hpp"
#include "layout/all.hpp"
#include "rendering/all.hpp"
#include "scaling/all.hpp"
#include "undo_redo/all.hpp"
#include "weights/all.hpp"

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

// NOTE: fwd declared these as such for build times.
static_assert(std::is_same_v< graph::vertex_descriptor, std::size_t >);

static_assert(
    std::is_same_v< layout_backend::layout_type::coord_type, double >);

static_assert(
    std::is_same_v< clustering_backend::cluster_type, unsigned long long >);

} // namespace presentation

#endif // PRESENTATION_DEF_HPP
