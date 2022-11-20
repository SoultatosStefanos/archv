// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef PRESENTATION_ALLFWD_HPP
#define PRESENTATION_ALLFWD_HPP

#include "clustering/allfwd.hpp"
#include "gui/allfwd.hpp"
#include "layout/allfwd.hpp"
#include "rendering/allfwd.hpp"
#include "scaling/allfwd.hpp"
#include "undo_redo/allfwd.hpp"
#include "weights/allfwd.hpp"

namespace presentation
{

class graph_interface;
class command_history;

class weights_backend;
class layout_backend;
class scaling_backend;
class degrees_backend;
class clustering_backend;

class background_renderer;
class graph_renderer;
class graph_collision_checker;

class menu_bar;
class weights_editor;
class layout_editor;
class scaling_editor;
class degrees_editor;
class clustering_editor;
class controls_hud;
class frames_hud;
class overlay_manager;
class popup_holder;

} // namespace presentation

#endif // PRESENTATION_ALLFWD_HPP
