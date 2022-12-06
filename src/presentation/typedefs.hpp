// Contains some presentation level typedefs.
// Soultatos Stefanos 2022

#ifndef PRESENTATION_TYPEDEFS_HPP
#define PRESENTATION_TYPEDEFS_HPP

#include "clustering/all.hpp"
#include "graph_interface.hpp"
#include "gui/all.hpp"
#include "layout/all.hpp"
#include "rendering/all.hpp"
#include "scaling/all.hpp"
#include "undo_redo/all.hpp"
#include "weights/all.hpp"

namespace presentation
{

/***********************************************************
 * Entities                                                *
 ***********************************************************/

using command_history = undo_redo::command_history;

/***********************************************************
 * Backends                                                *
 ***********************************************************/

using weights_backend = graph_interface::weights_backend_type;
using layout_backend = graph_interface::layout_backend_type;
using scaling_backend = graph_interface::scaling_backend_type;
using degrees_backend = rendering::degrees_ranked_backend;
using clustering_backend = graph_interface::clustering_backend_type;

/***********************************************************
 * Renderers                                               *
 ***********************************************************/

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

/***********************************************************
 * GUI                                                     *
 ***********************************************************/

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

#endif // PRESENTATION_TYPEDEFS_HPP
