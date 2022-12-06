// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef GUI_ALLFWD_HPP
#define GUI_ALLFWD_HPP

namespace gui
{

class overlay_manager;

class ui_adaptor;

class background_configurator;
class weights_editor;
class graph_configurator;
class gui_configurator;
class minimap_configurator;
class menu_bar;
class layout_editor;
class scaling_editor;
class degrees_editor;
class clustering_editor;

class frames_hud;
class controls_hud;

class structure_popup;
class popup_holder;

class gui_config;

using config_data = gui_config;
struct point;

} // namespace gui

#endif // GUI_ALLFWD_HPP
